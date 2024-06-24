<h1 align="center"> GPU-HPS-Kernel Library </h1>

<h4 align="center">Projeto da disciplina TEC 499 - Módulo Integrador Sistemas Digitais.</h4>

Biblioteca para o processador gráfico projetado, destinada a ser usada com o HPS (Hard Processor System) na plataforma DE1-SoC.

## Sumário
- [Contextualização](#contextualizacao)
- [Recursos utilizados](recursos-utilizados)
    - [Hardware](hardware)
    - [Software e Ferramentas](software-e-ferramentas)
    - [Bibliotecas e Dependências](bibliotecas-e-dependencias)
- [Modelagem e Organização da Arquitetura](modelagem-e-organizacao-da-arquitetura)
    - [Arquitetura do Processador Gráfico](arquitetura-do-processador-grafico)
    - [Instruções do Processador Gráfico](instrucoes-do-processador-grafico)
    - [Protocolo de Comunicação entre o HPS e o Processador Gráfico](protocolo-de-comunicacao-entre-o-hps-e-o-processador-grafico)
- [Módulo Kernel]()
- [Desenvolvimento da Biblioteca](desenvolvimento-da-biblioteca)
    - [Comunicação entre a biblioteca e o módulo kernel](comunicacao-entre-a-biblioteca-e-o-modulo-kernel)
    - [Algoritmo para implementação das funções da GPU](Algoritmo-para-implementacao-das-funcoes-da-gpu)
    - [Fluxograma de Exibição da Imagem no Monitor](fluxograma-para-exibicao-da-imagem-no-monitor)

- [Configurações de ambiente e execução](#configuracoes-de-ambiente-e-execucao)
- [Referências](#referências)

## Contextualização

Este projeto foi desenvolvido para atender aos requisitos do _Problema 2 - Barramento_ do componente curricular TEC499 - Módulo Integrador de Sistemas Digitais. O objetivo principal é conceber um Módulo Kernel que possibilite a comunicação entre o processador de propósito geral da placa DE1-SoC e o processador gráfico embarcado na FPGA da mesma placa. Além disso, o projeto exige o desenvolvimento de uma biblioteca para facilitar o acesso geral aos recursos do sistema e o envio de instruções para a GPU, garantindo uma integração eficiente e acessível entre os componentes de hardware e software.

## Recursos Utilizados

### Hardware

#### Placa DE1-SoC

A DE1-SoC é uma plataforma de desenvolvimento poderosa que combina um processador ARM de propósito geral com uma FPGA (Field-Programmable Gate Array) da Intel. Esta combinação permite o desenvolvimento e teste de sistemas embarcados complexos que requerem tanto processamento flexível quanto funcionalidades de hardware personalizáveis. Segue abaixo uma lista com os principais componentes da placa:

* Processador ARM Cortex-A9:
    * Dual-core ARM Cortex-A9.
    * Integrado no SoC (System on Chip) Cyclone V da Intel.
    * Executa um sistema operacional como Linux, possibilitando o desenvolvimento de software de alto nível.

* FPGA Cyclone V:
    * FPGA de alto desempenho que pode ser programada para implementar diversas funcionalidades de hardware.
    * Permite a criação de controladores personalizados, interfaces de comunicação, processadores gráficos embarcados, e outros módulos de hardware especializados.

* Memória:
    * 1 GB de memória DDR3 SDRAM para o processador ARM.
    * 64 MB de memória SDRAM para a FPGA.
    * Memória flash de 2 GB (para armazenamento de sistemas operacionais, bootloaders, e outros arquivos).

* Interfaces de Entrada/Saída:
    * Ethernet 10/100/1000.
    * USB 2.0 (Host e Device).
    * HDMI (saída de vídeo).
    * VGA (saída de vídeo).
    * Áudio (entrada e saída).
    * GPIO (General Purpose Input/Output).
    * Conectores de expansão para adicionar módulos periféricos.

* Periféricos:
    * Conversores A/D e D/A.
    * Interface de cartão SD.
    * LEDs, botões, e switches para interações básicas.

#### Monitor CRT (Cathode Ray Tube)

Os monitores CRT (Cathode Ray Tube) foram a tecnologia predominante para telas de computador e televisores durante grande parte do século XX. Embora agora tenham sido amplamente substituídos por tecnologias mais modernas como LCD, LED, e OLED, os CRTs desempenharam um papel crucial no desenvolvimento da eletrônica de exibição e foram utilizados durante o desenvolvimento de todo o projeto para a realização de testes.

### Software e Ferramentas

#### Compilador: 

- **GNU Compiler Collection (GCC)**:
  - Compilador robusto e altamente compatível com padrões de linguagem;
  - Suporte a várias linguagens, incluindo C, C++, Fortran, e mais;
  - Otimizações de código para melhorar o desempenho do software.

#### Ferramentas de Desenvolvimento:

- **Visual Studio Code**:
  - Editor de código-fonte leve com suporte a extensões;
  - Integração com Git para controle de versão e GitHub para colaboração;
  - Ferramentas avançadas de depuração e personalização da interface.

- **Git e GitHub**:
  - Sistema de controle de versão distribuído para rastreamento de alterações no código;
  - GitHub como plataforma de hospedagem de repositórios Git na nuvem;
  - Facilita colaboração, revisão de código e integração contínua/desenvolvimento contínuo.

#### Linguagem de Programação: 

- **C**:
  - Linguagem de programação de propósito geral com foco em eficiência e controle;
  - Altamente portável e adequada para desenvolvimento de sistemas e aplicações de baixo nível;
  - Base para muitas outras linguagens e bibliotecas importantes;
  - Suporte extenso da comunidade e ampla aplicação em sistemas operacionais, drivers e aplicativos críticos.

### Dependências

1. Instalação de Bibliotecas e Ferramentas para Desenvolvimento em C, como o CMake, ferramenta de build para projetos C/C++;

    ```bash
    sudo apt-get install cmake
    ```

2. Instalação ou atualização do compilador _GCC_ para realizar a compilação dos arquivos do projeto. Utilize o primeiro comando para verificar a versão do compilador ou o segundo caso a instalação dele não tenha sido realizada ainda;

    ```bash
    gcc --version
    ```

    ```bash
    sudo apt-get install gcc
    ```

3. Instalação de pacotes essenciais para desenvolvimento e gerenciamento de módulos de kernel em sistemas operacionais baseados em Debian;

    ```bash
    sudo apt-get install build-essential kmod
    ```

4. Instalação e configuração do _git_ para facilitar a manipulação do projeto.

    ```bash
    sudo apt-get install git
    git config --global user.name "Seu Nome"
    git config --global user.email "seu.email@example.com"
    ```

## Módulo de Kernel

O processador de propósito geral acessa a FPGA usando as pontes HPS-to-FPGA ou Lightweight HPS-to-FPGA. Essas pontes são mapeadas para regiões no espaço de memória do HPS. A fim de realizar a conexão do Processador Gráfico na FPGA com o Processador no HPS de forma mais facilitada, sem a necessidade de realizar o mapeamento de memória a cada acesso ao GPU, foi desenvolvido um módulo de kernel. Um módulo de kernel é um código que pode ser carregado e descarregado no kernel do sistema operacional Linux sem precisar recompilar ou reiniciar o sistema. Ele atua como uma extensão do kernel, adicionando novas funcionalidades ou suporte a novos dispositivos de hardware

O objetivo principal deste módulo de kernel é fornecer uma interface eficiente para escrita e leitura em dois barramentos, DATA A e DATA B, que conectam o processador HPS da placa DE1-SoC ao processador gráfico, facilitando a comunicação e melhorando o desempenho do sistema.

### Desenvolvimento do Módulo de Kernel

Para o devido funcionamento do módulo de kernel 

1. Definição da Interface
2. Drive de Caracteres
3.  

## Desenvolvimento da Biblioteca

Para estabelecer a comunicação entre a biblioteca e o módulo de Kernel, fazemos uso das instruções `write_data()`, `read_data` e `close_data`.

A função `write_data()`, como o nome pressupõe, efetua a escrita 

<img width="" src="https://github.com/yasmincsme/GraphicsProcessor-HPS-Kernel-Library/blob/docs/docs/write_data.jpg">


<img width="" src="https://github.com/yasmincsme/GraphicsProcessor-HPS-Kernel-Library/blob/docs/docs/read_data.jpg">

<img width="" src="https://github.com/yasmincsme/GraphicsProcessor-HPS-Kernel-Library/blob/docs/docs/close_data.jpg">

<img width="" src="https://github.com/yasmincsme/GraphicsProcessor-HPS-Kernel-Library/blob/docs/docs/instruction.jpg">

## Configurações de Ambiente e Execução

Primeiramente, conecte a placa à energia e aos cabos de rede e VGA. Além disso, certifique-se de que a GPU, para a qual a biblioteca foi modelada, está embarcada na FPGA da placa DE1-SoC.

Para acessar o material desenvolvido, clone o repositório disponível na plataforma GitHub utilizando o seguinte comando no terminal Linux:

```bash
git clone https://github.com/yasmincsme/GraphicsProcessor-HPS-Kernel-Library.git
```

Após clonar o repositório com sucesso, conecte-se à placa via SSH. Para isso, é necessário conhecer o IP da placa. Por exemplo, se o IP for `10.0.0.124`, use o seguinte comando:

```bash
ssh aluno@10.0.0.124
```

Em seguida, transfira a pasta clonada do seu computador para o sistema de arquivos da placa:

```bash
mv GraphicsProcessor-HPS-Kernel-Library/ [caminho do destino]
```

Para compilar e carregar o módulo do kernel desenvolvido, navegue até o diretório `src` do repositório e execute os seguintes comandos:

```bash
cd GraphicsProcessor-HPS-Kernel-Library/src/
make kernel load
```

O comando `make kernel` gerará os arquivos de compilação do módulo `gpp_data_bus.c`, e `load` o carregará no sistema como qualquer outro driver de dispositivo. Note que podem ocorrer problemas ao tentar carregar um módulo de kernel devido a permissões de dispositivo.

Se a operação for bem-sucedida, prossiga para compilar a biblioteca:

```bash
make build run
```

O comando `run` executará o arquivo gerado durante o processo de compilação.

---
