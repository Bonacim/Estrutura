#include "estrutura.h"

int main(int argc, char ** argv) {
    read_inputs();      // le os arquivos da estrutura, e arquivo de input
    prepare_environment();  // alocacao de memória, matrizes, inicializa o que precisar
    int iteration = 0;
    while( !converged() ) {
        calculate_forces();
        calculate_dispacements();
        render_window();
        iteration++;
        if(iteration > max_iteration) {
            print_failure();
//             printf("%d\n",iteration);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
