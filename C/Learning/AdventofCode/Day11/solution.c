#include "solution.h"

FILE *fp;
char *line;
size_t bufsize = 30;

int main(int argc, char *argv[]) {

    if (argc != 2) {fprintf(stderr, "[Usage]: ./solution <input_file.txt>\n"); return 1;}
    if ((line = malloc(bufsize * sizeof(char))) == NULL) {fprintf(stderr, "[Error]: Malloc problem\n"); return 1;}
    if ((fp = fopen(*++argv, "r")) == NULL) {fprintf(stderr, "[Error]: Could not open %s\n", *argv); return 1;}

    for (int num = 0; getline(&line, &bufsize, fp) >= 0; num++, num %= 7) {
        switch (num) {
            case 0: get_monkey(); break;
            case 1: get_items();  break;
            case 2: get_op();     break;
            case 3: get_test();   break;
            case 4: get_true();   break;
            case 5: get_false();  break;
        }
    }
    fclose(fp);
    free(line);

    for (int i = 0; i < ROUNDS; i++) {
        _round();
    }

    print_monkeys();
    return 0;
}

void send_item(int item, int monkey);
void remove_items(int monkey);

monkey_T monkeys[8];

void _round() {
    for (int monk = 0; monk < 8; monk++) {
        monkey_T *m = &monkeys[monk];
        for (int item = 0; item < m->next_item; item++) {
            m->inspected++;

            switch (m->op_type) {
                case '+': m->items[item] += m->op_num; break;
                case '*': m->items[item] *= m->op_num; break;
            }

            // m->items[item] /= 3;

            if (m->items[item] % m->test == 0)
                 send_item(m->items[item], m->T);
            else send_item(m->items[item], m->F);
        }

        remove_items(m->num);
    }
}

void send_item(int item, int monkey) {
    // reciever monkey
    monkey_T *r = &monkeys[monkey];
    r->items[(r->next_item)++] = item;
}

void remove_items(int monkey) {
    monkey_T *m = &monkeys[monkey];
    for (int i = 0; i < m->next_item; i++)
        m->items[i] = 0;

    m->next_item = 0;
}

int cur_monk;

void print_monkeys() {
    int part1 = 0;
    for (int i = 0; i < 8; i++) {
        printf("name: %d inspected: %d items: ", monkeys[i].num, monkeys[i].inspected);
        for (int j = 0; j < monkeys[i].next_item; j++) {
            printf("%d ", monkeys[i].items[j]);
        }
        printf("\n");
    }
}

void get_monkey() {
    cur_monk = line[7] - '0';
    monkeys[cur_monk].num = cur_monk;
    monkeys[cur_monk].next_item = 0;
    monkeys[cur_monk].inspected = 0;
}

void get_items() {
    for (int i = 16, item_index = 0; line[i] != '\0'; i++) {
        if (isDigit(line[i]) && !isDigit(line[i - 1])) {
            monkeys[cur_monk].items[monkeys[cur_monk].next_item++] = atoi(&line[i]);
        }
    }
}

void get_op() {
    monkeys[cur_monk].op_type = line[23];
    monkeys[cur_monk].op_num = atoi(&line[25]);
}

void get_test() { monkeys[cur_monk].test = atoi(&line[21]); }
void get_true() { monkeys[cur_monk].T = atoi(&line[29]); }
void get_false() { monkeys[cur_monk].F = atoi(&line[30]); }
