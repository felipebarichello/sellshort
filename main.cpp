#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>


typedef int data_t;
typedef std::vector<size_t> sequence_vector_t;
typedef sequence_vector_t(*sequence_rule_t)(size_t size);

// Uma sequência matemática.
// Eagerly evaluated
class Sequence {
public:
    sequence_rule_t rule;

    Sequence(sequence_vector_t (*rule)(size_t size)) {
        this->rule = rule;
    }

    sequence_vector_t generate(size_t size) {
        return this->rule(size);
    }
};

void ShellSort(data_t list[], size_t size, Sequence step_sequence, std::ofstream& file_out) {
    sequence_vector_t sequence_elements = step_sequence.generate(size);

    for (int sequence_index = sequence_elements.size() - 1; sequence_index >= 0; sequence_index--) {
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
            file_out << list[i] << " ";
        }

        file_out << " INCR=" << step << std::endl;
    }
}

sequence_vector_t shell_rule(size_t sorting_size) {
    sequence_vector_t sequence;
    
    for (size_t accumulator = 1; accumulator < sorting_size; accumulator *= 2) {
        sequence.push_back(accumulator);
    }

    return sequence;
}

sequence_vector_t knuth_rule(size_t sorting_size) {
    sequence_vector_t sequence;
    size_t current_element = 1;
   
    for (size_t power_of_three = 9; current_element < sorting_size; power_of_three *= 3) {
        sequence.push_back(current_element);                                                                                                    
        current_element = (power_of_three - 1)/2;
    }

    return sequence;
}

sequence_vector_t ciura_rule(size_t sorting_size) {
    sequence_vector_t sequence;
    size_t fixed_part[] { 1, 4, 10, 23, 57, 132, 701 };

    for (size_t i = 0; i < sizeof(fixed_part); i++) {
        if (sorting_size < fixed_part[i]) {
            return sequence;
        } else {
            sequence.push_back(fixed_part[i]);
        }
    }

    for (size_t accumulator = (size_t)(701 * 2.25); accumulator < sorting_size; accumulator = (size_t)(accumulator * 2.25)) {
        sequence.push_back(accumulator);
    }

    return sequence;
}

void get_input_vector(std::vector<int>& vector, std::ifstream& file_in) {
    size_t vector_size;
    data_t number;

    file_in >> vector_size;
    vector.resize(vector_size);

    for (size_t i = 0; i < vector_size; i++){
        file_in >> vector[i];
    }
}


struct BenchmarkConfig {
    std::vector<data_t>& data_vector;
    std::ofstream& file_out;
};

void sort_benchmark(BenchmarkConfig config, Sequence sequence, std::string sequence_name) {
    for (int number : config.data_vector) {
        config.file_out << number << " ";
    }

    config.file_out << " SEQ=" << sequence_name << std::endl;

    std::vector<data_t> data_copy = config.data_vector;
    ShellSort(&data_copy[0], config.data_vector.size(), sequence, config.file_out);
}

int main() {
    std::ifstream file_in;
    std::ofstream file_out;

    file_in.open("entradas/entrada-teste.txt");
    file_out.open("saida1.txt");

    if (!file_in.is_open()) {
        std::cerr << "Erro ao tentar abrir arquivo de entrada" << std::endl;
        return 1;
    }

    while (!file_in.eof()) {
        std::vector<data_t> data_vector;
        get_input_vector(data_vector, file_in);

        BenchmarkConfig config = {
            .data_vector = data_vector,
            .file_out = file_out,
        };

        sort_benchmark(config, Sequence(&shell_rule), "SHELL");
        sort_benchmark(config, Sequence(&knuth_rule), "KNUTH");
        sort_benchmark(config, Sequence(&ciura_rule), "CIURA");
    }
    
    file_in.close();
    file_out.close();

    return 0;
}
