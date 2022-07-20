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
	char			names[6][BUFF_SZ] = { 0 };
	uint64_t		sizes[6] = { 0 };
	uint64_t		count[6][2] = { 0 };
	int				src;
	uint64_t		i, sum, min, max, m_room, f_room, nb;
	int64_t			m_sz;
	Elf64_Phdr		*p_hdr, *p_txt;
	Elf64_Ehdr		*e_hdr;
	uint8_t			*m_src;

	if (argc != 2)
		quit_dir_fd_unmap(NULL, -1, NULL, 0, "./room_tester DIR");
	if (!(d = opendir(argv[1])))
		quit_dir_fd_unmap(NULL, -1, NULL, 0, "can't open source directory");
	for (min = -1, max = 0, sum = 0, i = 0, nb = 0;
			(dir = readdir(d)) != NULL; ++i) {
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
				if (e_hdr->e_type == ET_DYN && ((uint8_t *)e_hdr->e_ident)[4] == ELFCLASS64) {
					p_txt = NULL;
					for (i = 0, p_hdr = (Elf64_Phdr *)(m_src + e_hdr->e_phoff);
							!p_txt && i < e_hdr->e_phnum; ++i, ++p_hdr) {
						if (p_hdr->p_type == PT_LOAD && (p_hdr->p_flags & PF_X))
							p_txt = p_hdr;
					}
					m_room = p_hdr->p_vaddr - (p_txt->p_vaddr + p_txt->p_memsz);
					f_room = p_hdr->p_offset - (p_txt->p_offset + p_txt->p_filesz);
					if (m_room < min)
						min = m_room;
					if (m_room > max)
						max = m_room;
					sum += m_room;
					if (m_room < 50) {
						if (!count[0][0]) {
							strcpy(names[0], buff);
							sizes[0] = m_room;
						}
						++count[0][0];
					} else if (m_room < 100) {
						if (!count[1][0]) {
							strcpy(names[1], buff);
							sizes[1] = m_room;
						}
						++count[1][0];
					} else if (m_room < 500) {
						if (!count[2][0]) {
							strcpy(names[2], buff);
							sizes[2] = m_room;
						}
						++count[2][0];
					} else if (m_room < 1000) {
						if (!count[3][0]) {
							strcpy(names[3], buff);
							sizes[3] = m_room;
						}
						++count[3][0];
					} else if (m_room < 5000) {
						if (!count[4][0]) {
							strcpy(names[4], buff);
							sizes[4] = m_room;
						}
						++count[4][0];
					} else {
						if (!count[5][0]) {
							strcpy(names[5], buff);
							sizes[5] = m_room;
						}
						++count[5][0];
					}

					if (f_room < 50)
						++count[0][1];
					else if (f_room < 100)
						++count[1][1];
					else if (f_room < 500)
						++count[2][1];
					else if (f_room < 1000)
						++count[3][1];
					else if (f_room < 5000)
						++count[4][1];
					else
						++count[5][1];

					++nb;
				}
			}
			munmap(m_src, m_sz);
			close(src);
		}
	}
	if (!nb)
		quit_dir_fd_unmap(d, -1, NULL, 0, "no supported ELF file in this directory");
	printf(" file_sz        mem_sz\n");
	printf("---------  0B  --------\n");
	printf("|%7.2lf%%  |%7.2lf%%  |     ", count[0][1] * 100. / nb, count[0][0] * 100. / nb);
	printf("ex: %s (mem_sz=%ld)\n", names[0], sizes[0]);
	printf("--------- 50B  --------\n");
	printf("|%7.2lf%%  |%7.2lf%%  |     ", count[1][1] * 100. / nb, count[1][0] * 100. / nb);
	printf("ex: %s (mem_sz=%ld)\n", names[1], sizes[1]);
	printf("--------- 100B --------\n");
	printf("|%7.2lf%%  |%7.2lf%%  |     ", count[2][1] * 100. / nb, count[2][0] * 100. / nb);
	printf("ex: %s (mem_sz=%ld)\n", names[2], sizes[2]);
	printf("--------- 500B --------\n");
	printf("|%7.2lf%%  |%7.2lf%%  |     ", count[3][1] * 100. / nb, count[3][0] * 100. / nb);
	printf("ex: %s (mem_sz=%ld)\n", names[3], sizes[3]);
	printf("---------  1K  --------\n");
	printf("|%7.2lf%%  |%7.2lf%%  |     ", count[4][1] * 100. / nb, count[4][0] * 100. / nb);
	printf("ex: %s (mem_sz=%ld)\n", names[4], sizes[4]);
	printf("---------  5K  --------\n");
	printf("|%7.2lf%%  |%7.2lf%%  |     ", count[5][1] * 100. / nb, count[5][0] * 100. / nb);
	printf("ex: %s (mem_sz=%ld)\n", names[5], sizes[5]);
	printf("--------- Inf. --------\n");
	printf("\nbin = %ld, mem_sz: [min = %ld, max = %ld, mean = %ld]\n", nb, min, max, sum / nb);
	closedir(d);
	return(0);
}
