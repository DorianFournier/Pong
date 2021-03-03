#include "STM8.h"

struct sp_port* serialPort = NULL;


int STM8_connect(const char* serialPortName, int baudrate)
{
    struct sp_port_config* serialPortConfiguration;
    enum sp_return r;

    if (serialPort) sp_free_port(serialPort);

    if (sp_get_port_by_name(serialPortName, &serialPort) != SP_OK)
    {
        fprintf(stderr, "impossible de trouver le port\n");
        return 0;
    }

    if (sp_open(serialPort, SP_MODE_READ_WRITE) != SP_OK)
    {
        fprintf(stderr, "Impossible d'ouvrir le port série\n");
        return 0;
    }

    sp_new_config(&serialPortConfiguration);
    sp_set_config_baudrate(serialPortConfiguration, 115200);
    sp_set_config_parity(serialPortConfiguration, SP_PARITY_NONE);
    sp_set_config_bits(serialPortConfiguration, 8);
    sp_set_config_stopbits(serialPortConfiguration, 1);
    sp_set_config_flowcontrol(serialPortConfiguration, SP_FLOWCONTROL_NONE);

    if (sp_set_config(serialPort, serialPortConfiguration) != SP_OK)
    {
        fprintf(stderr, "Impossible de configurer le port serie");
        return 0;
    }

    sp_free_config(serialPortConfiguration);

    return 1;
}

int STM8_read(void* buffer, size_t bufferSize)
 {
    if (!serialPort) return 0;
    return sp_nonblocking_read(serialPort, buffer, bufferSize);
}

int STM8_write(const void* buffer, size_t bufferSize)
{
    if (!serialPort) return 0;
    return sp_nonblocking_write(serialPort, buffer, bufferSize) == SP_OK;
}

void STM8_disconnect()
{
    if (serialPort)
    {
        sp_close(serialPort);
        sp_free_port(serialPort);
        serialPort = NULL;
    }
}