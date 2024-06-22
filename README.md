<h1 align="center"> GPU-HPS-Kernel Library </h1>

<h4 align="center">Projeto da disciplina TEC 499 - Módulo Integrador Sistemas Digitais.</h4>

Biblioteca para o processador gráfico projetado, destinada a ser usada com o HPS (Hard Processor System) na plataforma DE1-SoC.

## Sumário
- [Visão Geral do Projeto](#visao-geral-do-projeto)
- [Referências](#referências)
- [Configuração de ambiente e execução](#configuracao-de-ambiente-e-execucao)

## Configuração de ambiente e execução

Para acessar o material desenvolvido, clone o repositório disponível na plataforma do GitHub usando o seguinte comando no terminal Linux:

```

git clone https://github.com/yasmincsme/GraphicsProcessor-HPS-Kernel-Library.git

```

Caso o processo de clonagem seja bem-sucedido, para compilar e carregar o módulo kernel desenvolvido, insira no terminal os comandos abaixo.

```

cd GraphicsProcessor-HPS-Kernel-Library/src/

```

```

make kernel load

```

A diretiva `make kernel` irá gerar os arquivos de compilação do módulo _gpp_data_bus.c_ e o `load` irá carregá-lo no computador como qualquer outro driver de dispositivo. \observe que podemos encontrar  problemas ao tentar carregar um módulo de kernel em um sistema Linux. Existem várias causas possíveis para esse problema e diversas maneiras de solucioná-lo.

Inicialmente, é necessário checar se o usuário tem as permissões necessárias para carregar módulos do kernel. Normalmente, isso requer privilégios de superusuário.

