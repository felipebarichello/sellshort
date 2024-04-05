#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>


#define TEST 2
#define PROCESSOR_NAME "Intel(R) Core(TM) i5-7400 CPU @ 3.00 GHz, 3001 Mhz, 4 Core(s), 4 Logical Processor(s)"

using steady_clock = std::chrono::steady_clock;

typedef int data_t;
typedef std::vector<size_t> sequence_vector_t;
typedef sequence_vector_t(*sequence_rule_t)(size_t size);


// Uma sequência matemática.
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

void ShellSort(data_t list[], size_t size, Sequence step_sequence, std::ofstream& file_out, std::chrono::duration<float>& execution_time) {
    sequence_vector_t sequence_elements = step_sequence.generate(size);

    #if TEST == 2
        steady_clock::time_point start_time = steady_clock::now();
    #endif
    
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
        
        #if TEST <= 1
            for (size_t i = 0; i < size; i++) {
                file_out << list[i] << " ";
            }

            file_out << " INCR=" << step << std::endl;
        #endif
    }

    #if TEST == 2
        execution_time = steady_clock::now() - start_time;
    #endif
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

    for (size_t i = 0; i < sizeof(fixed_part) / sizeof(size_t); i++) {
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

// Retorna booleano de se deve continuar
bool get_input_vector(std::vector<int>& vector, std::ifstream& file_in) {
    size_t vector_size;
    data_t number;

    file_in >> vector_size;

    if (file_in.fail()) {
        return false;
    }

    vector.resize(vector_size);

    for (size_t i = 0; i < vector_size; i++){
        file_in >> vector[i];
    }

    if (file_in.fail()) {
        return false;
    }

    return true;
}

struct BenchmarkConfig {
    std::vector<data_t>& data_vector;
    std::ofstream& file_out;
};

void sort_benchmark(BenchmarkConfig config, Sequence sequence, std::string sequence_name) {
    #if TEST <= 1
        for (int number : config.data_vector) {
            config.file_out << number << " ";
        }

        config.file_out << " SEQ=" << sequence_name << std::endl;
    #endif

    std::vector<data_t> data_copy = config.data_vector;
    std::chrono::duration<float> execution_time;
    ShellSort(&data_copy[0], config.data_vector.size(), sequence, config.file_out, execution_time);

    #if TEST == 2
        config.file_out << sequence_name << ", " << config.data_vector.size() << ", " << execution_time.count() * 1e3f << ", \"" << PROCESSOR_NAME << "\"" << std::endl;
    #endif
}

int main() {
    std::ifstream file_in;
    std::ofstream file_out;

    #if TEST == 0
        file_in.open("entradas/entrada0.txt");
        file_out.open("saida0.txt");
    #elif TEST == 1
        file_in.open("entradas/entrada1.txt");
        file_out.open("saida1.txt");
    #else
        file_in.open("entradas/entrada2.txt");
        file_out.open("saida2.txt");
    #endif

    if (!file_in.is_open()) {
        std::cerr << "Erro ao tentar abrir arquivo de entrada" << std::endl;
        return 1;
    }

    std::vector<data_t> data_vector;
    
    while (get_input_vector(data_vector, file_in)) {
        Sequence sequence = Sequence(shell_rule);

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
