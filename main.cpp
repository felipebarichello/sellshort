#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

/*
Procedimento insDiretaShellSort(C, s, f, h)
// C: vetor a ser ordenado
// s: tamanho do vetor
// h: incremento
// f: célula de início do incremento
início
    para j←(f+h) até s passo h faça
    início
            chave ← C[j]
            // inserir chave na seq ordenada C [1...j-1] 
            i ← (j–h)
            enquanto(i>0) e (C[i]>chave) faça
            início
                C[i+h] ← C[i]
                i ← (i-h)
            fim
        C[i+h] ← chave
    fim
fim


Procedimento shellSort(C, np, n)
// np = número de passos
// h = valor calculado do incremento
// j = define a célula de início de um segmento
// n = tamanho do vetor (qtd. de elementos)
início
    para p ← np decrescendo_até 1 faça
    início
        h ← 2p-1
        para j ← 1 até h faça
            insDiretaShellSort(C, n, j, h)
    fim
fim
*/


typedef int data_t;
typedef std::vector<size_t> sequence_t;

// Uma sequência matemática.
// Eagerly evaluated
class Sequence {
public:
    sequence_t (*rule)(size_t size);

    Sequence(sequence_t (*rule)(size_t size)) {
        this->rule = rule;
    }

    sequence_t generate(size_t size) {
        return this->rule(size);
    }
};

void ShellSort(data_t list[], size_t size, Sequence step_sequence, unsigned int stages, std::ofstream& file_out) {
    sequence_t sequence_elements = step_sequence.generate(stages);

    for (int sequence_index = stages - 1; sequence_index >= 0; sequence_index--) {
        size_t step = sequence_elements[sequence_index];

        for (size_t offset = 0; offset < step; offset++) {
            // Insertion sort interno em cada grupo (offset)
            for (size_t current_index = offset; current_index < size; current_index += step) {
                data_t new_key = list[current_index];
                size_t insertion_index = current_index;
                int check_index;

                // Encontrar local correto na sequência ordenada
                for (check_index = current_index - step; check_index >= 0 && list[check_index] > new_key; check_index -= step) {
                    list[insertion_index] = list[check_index];
                    insertion_index = check_index;
                }

                // Inserir elemento na sequência ordenada
                // `insertion_index` é o índice correto do elemento
                list[insertion_index] = new_key;
            }
        }

        for (size_t i = 0; i < size; i++) {
            file_out << " " << list[i];
        }

        file_out << " INCR=" << step << std::endl;
    }
}

// Não pode ter mais de 15 passos
sequence_t shell_rule(size_t elements) {
    sequence_t sequence;
    size_t accumulator = 1;

    for (size_t i = 0; i < elements; i++) {
        sequence.push_back(accumulator);
        accumulator *= 2;
    }

    return sequence;
}

void input_vector(std::vector<int>& vector, std::ifstream& file_in) {
    size_t vector_size;
    data_t number;

    file_in >> vector_size;
    vector.resize(vector_size);

    for (size_t i = 0; i < vector_size; i++){
        file_in >> vector[i];
    }
}

int main() {
    std::ifstream file_in;
    std::ofstream file_out;

    file_in.open("entradas/entrada-teste.txt");
    file_out.open("output/saida1.txt");

    if (!file_in.is_open()) {
        std::cerr << "Erro ao tentar abrir arquivo de entrada" << std::endl;
        return 1;
    }

    std::vector<int> data;
    
    while (!file_in.eof()) {
        input_vector(data, file_in);

        for (int number : data) {
            file_out << " " << number;
        }

        file_out << " SEQ=SHELL" << std::endl;

        Sequence sequence = Sequence(shell_rule);
        
        ShellSort(&data[0], data.size(), shell_rule, 4, file_out);
    }

    file_in.close();
    file_out.close();

    return 0;
}