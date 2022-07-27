#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include <elf.h>
#include <stdlib.h>

#define BUFF_SZ		1024

extern uint8_t		sc;
extern uint8_t		sc_end;
extern uint8_t		sc_data;
extern uint8_t		sc_data_end;

void	quit_dir_fd_unmap(DIR *dir, int fd, uint8_t *m, int64_t m_sz, char *msg) {
	if (dir)
		closedir(dir);
	if (fd >= 0)
		close(fd);
	if (m)
		munmap(m, m_sz);
	if (msg) {
		fprintf(stderr, "Error: %s\n", msg);
		exit(42);
	} else
		exit(0);
}

int		main(int argc, char **argv) {
	DIR				*d;
	struct dirent	*dir;
	char			buff[BUFF_SZ];
	int				src;
	uint64_t		i, code_sz, data_sz, load_sz, success, nb;
	int64_t			m_sz;
	Elf64_Phdr		*p_hdr, *p_txt;
	Elf64_Ehdr		*e_hdr;
	uint8_t			*m_src;

	if (argc != 2)
		quit_dir_fd_unmap(NULL, -1, NULL, 0, "./room_tester DIR");
	if (!(d = opendir(argv[1])))
		quit_dir_fd_unmap(NULL, -1, NULL, 0, "can't open source directory");

	code_sz = &sc_end - &sc;
	data_sz = &sc_data_end - &sc_data;
	load_sz = data_sz + code_sz;

	printf("Payload size: %ld\n", load_sz);
	for (i = 0, success = 0, nb = 0; (dir = readdir(d)) != NULL; ++i) {
		if (dir->d_type == DT_REG) {
			sprintf(buff, "%s/%s", argv[1], dir->d_name);
			if ((src = open(buff, O_RDONLY)) < 0)
				quit_dir_fd_unmap(d, src, NULL, 0, "can't open source file");
			if ((m_sz = lseek(src, 0, SEEK_END)) < 0 || lseek(src, 0, SEEK_SET) < 0)
				quit_dir_fd_unmap(d, src, NULL, 0, "can't seek into source file");
			if ((m_src = mmap(NULL, m_sz, PROT_READ | PROT_WRITE,
							MAP_PRIVATE, src, 0)) == MAP_FAILED)
				quit_dir_fd_unmap(d, src, NULL, 0, "can't map source file into memory");
			if (!strncmp((char *)m_src, "\x7f" "ELF", 4)) {
				e_hdr = (Elf64_Ehdr *)m_src;
				if ((e_hdr->e_type == ET_DYN || e_hdr->e_type == ET_EXEC)
						&& ((uint8_t *)e_hdr->e_ident)[4] == ELFCLASS64) {
					p_txt = NULL;
					for (i = 0, p_hdr = (Elf64_Phdr *)(m_src + e_hdr->e_phoff);
							!p_txt && i < e_hdr->e_phnum; ++i, ++p_hdr) {
						if (p_hdr->p_type == PT_LOAD && (p_hdr->p_flags & PF_X))
							p_txt = p_hdr;
					}
					if (load_sz <= p_hdr->p_vaddr - (p_txt->p_vaddr + p_txt->p_memsz)) {
						if (load_sz > p_hdr->p_offset - (p_txt->p_offset + p_txt->p_filesz))
							printf("%s\n", buff);
						++success;
					}
					++nb;
				}
			}
			munmap(m_src, m_sz);
			close(src);
		}
	}
	printf("Injection success rate: %ld/%ld (%.2lf%%)\n",
			success, nb, nb ? success * 100. / nb : 100);
}
