#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

size_t get_file_size(const char* f_name)
{
    struct stat st;
    if (stat(f_name, &st) != 0)
    {
        perror("stat");
        exit(1);
    }
    return st.st_size;
}

int main(int argc, char* argv[])
{
    CPU cpu = {.PC = 0, .REG = {0}, .fetch = fetch, .execute = execute, .step = step};
    CPU* cpu_ptr = &cpu;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <rom>\n", argv[0]);
        return 1;
    }

    const char* file_name = argv[1];
    size_t file_size = get_file_size(file_name);

    if (file_size > MEM_SIZE)
    {
        fprintf(stderr, "ROM too large for memory\n");
        return 1;
    }

    uint8_t* buffer = malloc(file_size);
    if (!buffer)
    {
        perror("malloc");
        return 1;
    }

    FILE* fd = fopen(file_name, "rb");
    if (!fd)
    {
        perror("fopen");
        return 1;
    }

    size_t read = fread(buffer, 1, file_size, fd);
    fclose(fd);

    if (read != file_size)
    {
        fprintf(stderr, "Failed to read ROM\n");
        return 1;
    }
    memcpy(DRAM, buffer, file_size);
    free(buffer);

    while (!cpu_ptr->halted && cpu_ptr->PC + 4 <= MEM_SIZE)
    {
        cpu_ptr->pc_set = 0;
        uint32_t instr = cpu_ptr->fetch(cpu_ptr);
        cpu_ptr->execute(instr, cpu_ptr);
        if (!cpu_ptr->pc_set && !cpu_ptr->halted)
        {
            cpu_ptr->step(cpu_ptr);
        }
        cpu_ptr->REG[0] = 0;
    }

    return 0;
}
