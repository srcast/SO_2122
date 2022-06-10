#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> /* chamadas ao sistema: defs e decls essenciais */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_CREAT, O_* */
#include <stdio.h> //para utilizar o perror
#include <string.h>
#include <stdlib.h>


typedef struct person {
	char name[20];
	int age;
} Person;    // 20 bytes + 4 bytes(int) = 24 bytes de tamanho


// para usar *Person depois tinhamos que alocar a memoria para esse apontador

int main(int argc, char const *argv[]) {
	
	Person p1, p2, p3;

	int person_fd, bytes_read;

    if(argc != 4){
        puts("Input inválido");
        return 1;
    }

 switch(*(argv[1] + 1)){
 	
 	case 'i':
            person_fd = open("registos.bin", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR, 0640);

			strcpy(p1.name, argv[2]);
			p1.age = atoi(argv[3]);

            // determinar o tamanho do ficheiro --> saquei da net
            struct stat st; 
            fstat(person_fd, &st);
            off_t tamanhoFicheiro = st.st_size;

            int pos = (int)tamanhoFicheiro / sizeof(struct person);

            printf("registo %d\n", (pos + 1));

			if(write(person_fd, &p1, sizeof(struct person)) < 1) {
                puts("Nao foi possivel escrever no ficheiro.");
            }
            break;

    case 'u':
            person_fd = open("registos.bin", O_RDWR); // como tinha o append estava sempre a escrever no fim
            // para dar update e escrever no fim tem que ser duas opcoes de abertura do ficheiro
    		
            //lseek(person_fd, 0, SEEK_SET); //como está a ser aberto novamente nao preciso do lseek

			strcpy(p1.name, argv[2]);
			p1.age = atoi(argv[3]);

			while(read(person_fd, &p2, sizeof(struct person)) > 0) {
				if(strcmp(p2.name, p1.name) == 0){
					lseek(person_fd, - sizeof(struct person), SEEK_CUR);
					write(person_fd, &p1, sizeof(struct person));
                    break; // atualiza só o primeiro que encontrar
				}
            }
            break;
    case 'o':
            person_fd = open("registos.bin", O_RDWR);

            int registo = atoi(argv[2]);
            int age = atoi(argv[3]);

            lseek(person_fd, 0, SEEK_SET);
            lseek(person_fd, (registo - 1) * sizeof(struct person), SEEK_CUR); // ex: atualiza o registo 2 na posiçao 1

            if(read(person_fd, &p2, sizeof(struct person)) > 0) {
                    p2.age = age;
                    lseek(person_fd, - sizeof(struct person), SEEK_CUR);
                    write(person_fd, &p2, sizeof(struct person));
            } 
            else perror("leitura invalida opcao -o\n");
            break;

    default:
            puts("OPCAO INVALIDA");
            return 1; //caso nao seja uma opcao valida, termina o programa
            break;

 }

 //lseek(person_fd, 0, SEEK_SET);

 person_fd = open("registos.bin", O_RDONLY);

	while((bytes_read = read(person_fd, &p3, sizeof(struct person))) > 0){

		printf("Nome: %s - %d\n", p3.name, p3.age);

	}

	perror("read");


 	close(person_fd);


	// ls -la para ver os registos

	//hexdump -C person.bin para ver o que tem o ficheiro


	return 0;
}