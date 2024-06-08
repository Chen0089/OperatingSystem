#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <yaml.h>

#define MAX_COMMAND_LENGTH 100
#define USER_FILE_PATH "user.yml"

// 结构体表示用户信息
typedef struct {
    char name[MAX_COMMAND_LENGTH];
    char password[MAX_COMMAND_LENGTH];
} User;

// 读取用户文件并返回用户信息
User read_user_file() {
    User user;
    FILE *file = fopen(USER_FILE_PATH, "r");
    if (file) {
        yaml_parser_t parser;
        yaml_token_t token;
        yaml_parser_initialize(&parser);
        yaml_parser_set_input_file(&parser, file);
        
        int is_name = 0, is_password = 0;
        while (yaml_parser_scan(&parser, &token)) {
            if (token.type == YAML_SCALAR_TOKEN) {
                if (is_name) {
                    strncpy(user.name, (char *)token.data.scalar.value, MAX_COMMAND_LENGTH);
                    is_name = 0;
                } else if (is_password) {
                    strncpy(user.password, (char *)token.data.scalar.value, MAX_COMMAND_LENGTH);
                    is_password = 0;
                } else {
                    if (strcmp((char *)token.data.scalar.value, "name") == 0) {
                        is_name = 1;
                    } else if (strcmp((char *)token.data.scalar.value, "password") == 0) {
                        is_password = 1;
                    }
                }
            }
            yaml_token_delete(&token);
        }
        yaml_parser_delete(&parser);
        fclose(file);
    }
    return user;
}

// 验证用户登录信息
int validate_user(User user) {
    char input_name[MAX_COMMAND_LENGTH];
    char input_password[MAX_COMMAND_LENGTH];
    
    printf("Username: ");
    fflush(stdout);
    fgets(input_name, sizeof(input_name), stdin);
    input_name[strcspn(input_name, "\n")] = 0;
    
    // 如果密码为空白，则不需要输入密码
    if (strlen(user.password) == 0) {
        return (strcmp(user.name, input_name) == 0);
    }
    
    printf("Password: ");
    fflush(stdout);
    fgets(input_password, sizeof(input_password), stdin);
    input_password[strcspn(input_password, "\n")] = 0;
    
    return (strcmp(user.name, input_name) == 0 && strcmp(user.password, input_password) == 0);
}

// 闪烁线程
void *blink_thread(void *arg) {
    int blink = 1;
    while (1) {
        if (blink) {
            printf("\033[H>"); // 将光标移动到起始位置输出“>”
        } else {
            printf("\033[H \033[K"); // 将光标移动到起始位置清除字符
        }
        fflush(stdout); // 刷新输出缓冲区
        blink = !blink; // 切换闪烁状态
        sleep(1);
    }
    return NULL;
}

// 解析用户命令
void parse_command(char *command) {
    if (strcmp(command, "help") == 0) {
        printf("This is a simple command line system.\n");
        printf("Available commands:\n");
        printf("1. help - Display this help message\n");
        printf("2. greet - Say hello\n");
        printf("3. exit - Exit the program\n");
    } else if (strcmp(command, "greet") == 0) {
        printf("Hello! Welcome to the command line system.\n");
    } else if (strcmp(command, "exit") == 0) {
        printf("Exiting...\n");
        // 退出程序
    } else {
        printf("Command not recognized. Type 'help' for available commands.\n");
    }
}

int main() {
    pthread_t blink_tid;
    
    // 创建闪烁线程
    pthread_create(&blink_tid, NULL, blink_thread, NULL);
    
    // 读取用户文件获取用户信息
    User user = read_user_file();
    
    // 用户登录验证
    if (!validate_user(user)) {
        printf("Invalid username or password. Exiting...\n");
        return 1;
    }
    
    printf("System: Welcome, %s!\n", user.name);
    
    char command[MAX_COMMAND_LENGTH];
    
    while (1) {
        printf("> ");
        fflush(stdout); // 刷新输出缓冲区
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;
        
        parse_command(command);
        
        if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    
    pthread_cancel(blink_tid); // 取消闪烁线程
    
    return 0;
}
