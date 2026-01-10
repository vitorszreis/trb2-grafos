#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "==================================" << std::endl;
    std::cout << "  DC-MST - Trabalho de Grafos    " << std::endl;
    std::cout << "==================================" << std::endl;
    
    if (argc < 2) {
        std::cout << "\nUso: " << argv[0] << " <algoritmo> <parametros>" << std::endl;
        std::cout << "\nAlgoritmos disponíveis:" << std::endl;
        std::cout << "  guloso <instancia>" << std::endl;
        std::cout << "  grasp <instancia> <alfa> <iteracoes>" << std::endl;
        std::cout << "  reativo <instancia> <iteracoes> <tamanho_bloco>" << std::endl;
        return 1;
    }
    
    std::cout << "\n Compilação bem-sucedida!" << std::endl;
    std::cout << " Estrutura básica funcionando." << std::endl;
    
    return 0;
}