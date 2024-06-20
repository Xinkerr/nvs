#include <nvs.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static int _erase(uint32_t offset, uint32_t size)
{
    //TODO: flash erase
    return 0;
}

static int _write(uint32_t offset, const void* pdata, uint32_t len)
{
    //TODO: flash write
    return 0;
}

static int _read(uint32_t offset, void* pdata, uint32_t len)
{
    //TODO: flash read
    return 0;
}

int main( void )
{
    const uint8_t fs_erase_val[1] = {0xff};
	struct nvs_fs fs;
	struct nvs_ops ops = 
    {
        .read = _read,
        .write = _write,
        .erase = _erase,
    };
    struct nvs_fs_property property =
    {
        .address = 0,
        .sector_size = 4096,
        .sector_count = 2,
        .write_block_size = 1,
        .erase_value = fs_erase_val,
        .erase_value_len = 1,
    };

	int ret = nvs_init(&fs, &property, &ops);
    printf("nvs_init:%d\r\n", ret);

    uint32_t reboot_cnt = 0;
    nvs_read(&fs, 0, &reboot_cnt, 4);
    printf("reboot cnt:%u\r\n", reboot_cnt);
    reboot_cnt++;
    nvs_write(&fs, 0, &reboot_cnt, 4);

    while( 1 )
    {
        
    }
}
