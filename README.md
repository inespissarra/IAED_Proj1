# IAED Sistema de Gestão de Tarefas

- Compilar: gcc -Wall -Wextra -Werror -ansi -pedantic -o proj1 *.c
- Executar: ./proj1

## Comandos
- q: termina o programa
- t <duração> <descrição>: adiciona uma nova tarefa ao sistema
- l *<_id_>: lista as tarefas
- n <duração>: avança o tempo do sistema
- u [<_utilizador_>]: adiciona um utilizador ou lista todos os utilizadores
- m <_id_> <_utilizador_> <_atividade_>: move uma tarefa de uma atividade para outra
- d <_atividade_>: lista todas as tarefas que estejam numa dada atividade
- a [<_atividade_>]: adiciona uma atividade ou lista todas as atividades