#pragma once

#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include <err.h>
#include <errno.h>
#include <stdint.h>

#include <vector>
#include <iostream>



size_t min(size_t x, size_t y) { return x > y ? y : x; }

// https://stackoverflow.com/questions/63208333/using-memfd-create-and-fexecve-to-run-elf-from-memory
/**
 * @param len != 0
 */
void fdput(int fd, const uint8_t* str, size_t len)
{
    size_t cnt = 0;
    do {
        ssize_t result = write(fd, str + cnt, min(len - cnt, 0x7ffff000));
        if (result == -1) {
            if (errno == EINTR)
                continue;
            err(1, "%s failed", "write");
        }
        cnt += result;
    } while (cnt != len);
}


class MemFile
{
    const char* name;
    int fd;

public:

    MemFile(const char* _name) : name(_name), fd(memfd_create(_name, 0)) {
        if (fd == -1) err(1, "%s failed", "memfd_create");
    }

    MemFile& write(const std::vector<uint8_t>& data) {
        fdput(fd, data.data(), data.size());
        return *this;
    }

    int execute() {
        switch (auto pid = fork())
        {
        case -1:
            std::cout << "fork() failed" << std::endl;
            exit(1);
        case 0: {
            const char * const argv[] = {name, NULL};
            const char * const envp[] = {NULL};
            fexecve(fd, (char * const *) argv, (char * const *) envp);
        } 
        default:
            int status;
            waitpid(pid, &status, 0);
        }
        return 0;
    }
};