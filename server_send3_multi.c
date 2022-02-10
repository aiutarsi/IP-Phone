#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define N 512
#define NUM_THREAD 2

/*
struct reading {
    short read_data[N];
    int socket;
    int fd_output;
};

struct sending {
    short send_data[N];
    int socket;
    FILE *gid;
    int fd_input;
};
*/

struct data {
    short *datas;
    int socket;
    FILE *gid;
    int fd;
    FILE *fp;
};

void* func_read(void *arg) {
    struct data *read_info = arg;
    while(1) {
        int n_read = read(read_info->socket, read_info->datas, N*2);
        if (n_read > 0) {
            write(1, read_info->datas, n_read);
            write(read_info->fd, read_info->datas, n_read);
            fprintf(read_info->fp, "-1024byte read\n");
        }
        else if (n_read == -1) {
            fprintf(read_info->fp, "read_out\n");
            exit(1);
        }
        else if (n_read == 0) {
            fprintf(read_info->fp, "read_break\n");
            break;
        }
    }
    return NULL;
}
void* func_send(void *arg) {
    struct data *send_info = arg;
    while(1) {
        int n_send = fread(send_info->datas, sizeof(short), N, send_info->gid);
        if (n_send > 0) { 
            write(send_info->socket, send_info->datas, n_send*2);
            write(send_info->fd, send_info->datas, n_send*2);
            fprintf(send_info->fp, "-1024byte send\n");
        }
        else if (n_send == -1) {
            fprintf(send_info->fp, "send_out\n");
            exit(1);
        }
        else if (n_send == 0) {
            fprintf(send_info->fp, "send_break\n");
            break;
        }
    }
    return NULL;   
}

/*
void *Read_Data_From_Client(void *arg);
void *Write_Data_To_Client(void *arg);

void *Read_Data_From_Client(void *arg) {
    struct reading *read_info = arg;
    while(1) {
        int n_read = read(read_info->socket, read_info->read_data, N*2);
        if (n_read > 0) {
            write(1, read_info->read_data, n_read);
            write(read_info->fd_output, read_info->read_data, n_read);
        }
        else if (n_read == -1) {
            die("read");
        }
        else if (n_read == 0) {
            break;
        }
    }
}

void *Write_Data_To_Client(void *arg) {
    struct sending *send_info = arg;
    while(1) {
        int n_send = fread(send_info->send_data, sizeof(short), N, send_info->gid);
        if (n_send > 0) {
            write(send_info->socket, send_info->send_data, n_send*2);
            write(send_info->fd_input, send_info->send_data, n_send*2);
        }
        else if (n_send == -1) {
            die("send");
        }
        else if (n_send == 0) {
            break;
        }
    }
}
*/

int main(int argc, char *argv[]) {
    FILE *fp;
    fp = fopen("server_exe_log.txt", "w");
    
    if (argc != 2) {
        fprintf(fp, "Incorrect argument\n");
        exit(1);
    }

    fprintf(fp, "Read input\n");

    const short port = (short) atoi(argv[1]);

    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s == -1) {
        fprintf(fp, "Making Socket\n");
        exit(1);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        fprintf(fp, "Cannot Bind\n");
        exit(1);
    }

    if (listen(s, 10) == -1) {
        fprintf(fp, "Cannot Listen\n");
        exit(1);
    }

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int ss = accept(s, (struct sockaddr *)&client_addr, &len);
    if (ss == -1) {
        fprintf(fp, "Cannot Accept\n");
        exit(1);
    }
    close(s);

    FILE *gid;
    gid = popen("rec -t raw -b 16 -c 1 -e s -r 48000 -", "r");
    if (gid == NULL) {
        fprintf(fp, "popen : Cannot Open\n");
        exit(1);
    }
    
    int fd_input = open("server_input_log.raw", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd_output = open("server_output_log.raw", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    //Read data from stdin and Write data to socket s
    short send_data[N];
    short read_data[N];

    pthread_t t[NUM_THREAD];

    struct data d[NUM_THREAD];

    d[0].datas = read_data;
    d[0].socket = ss;
    d[0].gid = gid;
    d[0].fd = fd_output;
    d[0].fp = fp;

    d[1].datas = send_data;
    d[1].socket = ss;
    d[1].gid = gid;
    d[1].fd = fd_input;
    d[1].fp = fp;

    //struct reading read = {read_data, ss, fd_output};
    //struct sending send = {send_data, ss, gid, fd_input};

    //pthread_create(&t[0], NULL, Read_Data_From_Client, &read);
    //pthread_create(&t[1], NULL, Write_Data_To_Client, &send);

    pthread_create(&t[0], NULL, func_read, &d[0]);
    pthread_create(&t[1], NULL, func_send, &d[1]);

    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);

    if (shutdown(ss, SHUT_WR) == -1) {
        fprintf(fp, "Cannot Shutdown\n");
        exit(1);
    }

    close(fd_input);
    close(fd_output);
    pclose(gid);
    close(ss);
    fclose(fp);
}
