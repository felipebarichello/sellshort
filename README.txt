# Estrutura de arquivos

## bubbles_and_buckets/

Contém os arquivos do problema bônus do Bubbles and Buckets.
É um projeto Cargo (Rust):
- `src/` contém os arquivos de código fonte;
- `Cargo.toml` e `Cargo.lock` são arquivos do pacote Cargo;
- `comprovante.png` é uma _screenshot_ da aceitação no site beecrowd.

## entradas/

Os arquivos de entrada para o programa principal.
- `entrada0.txt` é o arquivo de exemplo contido dentro da especificação do trabalho.
- `entrada1.txt` e `entrada2.txt` são os arquivos fornecidos para os testes de fato.

## integrantes.txt

Os nomes dos integrantes do grupo.

## main.cpp

O código fonte do programa principal, escrito em C++.
Gera `saidaN.txt` para os arquivos `entradaN.txt`.
Possui dois parâmetros que podem ser modificados antes da compilação do código:
- `TEST` indica qual arquivo de entrada `entrada{TEST}.txt` será lido.
- `PROCESSOR_NAME` define o nome do processador que será utilizado em `saida2.txt`.
