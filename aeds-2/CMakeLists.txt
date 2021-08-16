cmake_minimum_required(VERSION 3.19)
project(TP)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(TP
        arvore_binaria.c
        arvore_binaria.h
        busca_bin.c
        funcionario.c
        funcionarios.h
        intercalacao.c
        intercalacao.h
        lista.c
        lista.h
        main.c
        manipulacao_arquivos.c
        manipulacao_arquivos.h
        ordenacao_busca.c
        ordenacao_busca.h
        particoes.c
        particoes.h
        pilha.c
        pilha.h
        selecao_natural.cpp)
