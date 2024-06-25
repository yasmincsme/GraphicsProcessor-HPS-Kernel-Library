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

## Modelagem e Organização da Arquitetura

A primeira etapa do processo de renderização consistiu na inicialização de duas memórias responsáveis por armazenar os bits de cores RGB dos sprites e do background da tela. A segunda etapa corresponde ao controle de impressão dos elementos da tela, como os sprites. O padrão gráfico escolhido foi o VGA com uma resolução de 640x480 pixels. Como a varredura de um monitor VGA é realizada da esquerda para a direita e de cima para baixo, ao finalizar a varredura de toda a área ativa da tela, os sprites podem ser atualizados, de forma que, no próximo frame, eles sejam redesenhados de acordo com as novas definições.

Cada jogo implementado por meio da arquitetura proposta deve ser programado utilizando a linguagem C através de um processador de propósito geral. Como as informações dos sprites são armazenadas a nível de hardware, as atualizações são feitas através de instruções (comandos) enviadas ao módulo gráfico responsável pelo processo de renderização dos jogos. O processo de renderização é independente e executa todo o controle sem a necessidade de intervenções via software. Com isso, o código em linguagem C ficará responsável por definir a lógica do jogo e enviar os comandos de atualização das informações pertencentes aos objetos a serem renderizados.

Com base nesse princípio de funcionamento, temos a modelagem da arquitetura apresentada na Fig. 3. Sua estrutura consiste em um processador de propósito geral, duas FIFOs (First In First Out), uma PLL (Phase Locked Loop) e um Processador Gráfico. Para atuar como processador de propósito geral foi escolhido o Nios II, um processador softcore RISC de 32 bits com arquitetura Harvard desenvolvido pela empresa Altera. Sua função é executar o código-fonte em linguagem C dos jogos que serão programados. O Processador Gráfico é responsável por gerenciar o processo de renderização dos jogos e executar um conjunto de instruções que permitem inicialmente mover e controlar sprites, como também modificar o layout do background da tela e renderizar polígonos do tipo quadrado e triângulo. As principais saídas do Processador Gráfico consistem nos sinais de sincronização horizontal (h sync) e vertical (v sync) do monitor VGA, e os bits de cores RGB. Como o Nios II e o Processador Gráfico possuem frequências de clock e controle distintos, as FIFOs são utilizadas como dispositivos intermediários para comunicação. A PLL é responsável por gerar as frequências de clock necessárias para o correto funcionamento da arquitetura. O Nios II armazena nas FIFOs todas as instruções que devem ser executadas pelo Processador Gráfico. O módulo Gerador de Pulso é responsável por gerar um único pulso de escrita em sincronização com o sinal wrclk; por consequência, os dados de instrução presentes nos barramentos dataA e dataB serão armazenados nas FIFOs uma única vez. Cada FIFO possui inicialmente a capacidade de armazenar 16 palavras de 32 bits. Quando o sinal wrfull está em nível lógico alto, significa que as FIFOs alcançaram sua capacidade máxima. Desta forma, o circuito interno de proteção das FIFOs é ativado automaticamente para evitar possíveis overflows. As FIFOs facilitam o processo de modelagem de novas instruções, não sendo necessário a inclusão de novos barramentos e/ou dispositivos intermediários entre o Nios II e o Processador Gráfico.

### Arquitetura do Processador Gráfico

É possível observar a representação da estrutura interna deste módulo na Fig. 4. Inicialmente, temos a Unidade de Controle, que consiste em uma Máquina de Estados responsável por gerenciar o processo de leitura, decodificação e execução das instruções recebidas. O Banco de Registradores armazena temporariamente as informações (coordenadas, offset de memória, e um bit de ativação) associadas a cada sprite. São 32 registradores no total, sendo o primeiro reservado para o armazenamento da cor de background da tela e os outros 31 reservados aos sprites. Com isso, o Processador Gráfico consegue administrar o uso de 31 sprites em um mesmo frame de tela. O Módulo de Desenho é responsável por gerenciar o processo de desenho dos pixels no monitor VGA. O Controlador VGA é responsável por gerar os sinais de sincronização v sync e h sync da VGA, além de fornecer as coordenadas x e y do processo de varredura do monitor.

Considerando os tempos de sincronização vertical e horizontal, cada tela é impressa a cada 16,768 ms. Logo, temos uma taxa de aproximadamente 60 frames por segundo. A Memória de Sprites armazena o bitmap para cada sprite. Sua dimensão é ajustada conforme necessário para suportar os diferentes tamanhos de sprites usados no jogo.

A estrutura interna deste módulo é representada na Fig. 4. A Memória de Sprites consiste em 12.800 palavras de 9 bits, com 3 bits para cada componente RGB. Cada sprite deve ter o tamanho de 20x20 pixels, ocupando assim 400 posições de memória. Dessa forma, é possível armazenar 32 diferentes sprites para uso em um jogo. A Memória de Background é utilizada para modificar pequenas partes do background e consiste em 4.800 palavras de 9 bits. A inicialização das memórias é realizada durante o processo de síntese do projeto no software Quartus.

Na Figura 4, também encontra-se um Co-Processador, responsável por gerenciar a construção de polígonos convexos do tipo quadrado e triângulo. Esses polígonos serão renderizados na tela de um monitor VGA em conjunto com os sprites e o background.

### Instruções do Processador Gráfico

Nesta primeira versão do projeto, o Processador Gráfico possui as seguintes instruções:

1. **Escrita no Banco de Registradores (WBR):** 
   Essa instrução é responsável por configurar os registradores que armazenam as informações dos sprites e a cor base do background. Como essa cor base é armazenada no primeiro registrador do Banco, a instrução WBR segue a estrutura apresentada na Fig. 8. As posições com valor 0 representam posições não utilizadas. Para configurar um sprite, segue-se a estrutura apresentada na Fig. 9. O campo opcode define qual instrução será executada pelo Processador Gráfico. Para esta instrução, o valor é configurado em 0000. A escolha do sprite é feita através do campo offset, que indica a localização do sprite na memória. O campo registrador é utilizado para definir em qual registrador os parâmetros de impressão serão armazenados. A posição do sprite é definida através das coordenadas X e Y. O campo sp permite habilitar/desabilitar o desenho de um sprite na tela. Na Fig. 8, os campos R, G e B configuram as componentes RGB da cor base do background.

   **Figura 8:** Uso das Instruções WBR para modificação da cor base do background.
   
   **Figura 9:** Uso das Instruções WBR para configurar um sprite.

2. **Escrita na Memória de Sprites (WSM):** 
   Essa instrução armazena ou modifica o conteúdo presente na Memória de Sprites (Fig. 10). O campo opcode é semelhante à instrução anterior, no entanto, seu valor é configurado em 0001. O campo endereço de memória especifica qual local da memória será alterado. Os campos R, G e B definem as novas componentes RGB para o local desejado.

   **Figura 10:** Representação da Instrução WSM.

3. **Escrita na Memória de Background (WBM):**
   Essa instrução armazena ou modifica o conteúdo presente na Memória de Background. Sua função é configurar valores RGB para o preenchimento de áreas do background. Seus campos são semelhantes aos da instrução WSM (Fig. 10), com a única diferença no campo endereço de memória, que possui tamanho de 12 bits. O valor do opcode é configurado como 0010. O background é dividido em pequenos blocos de 8x8 pixels, e cada endereço de memória corresponde a um bloco. Com a resolução de 640x480 pixels, temos uma divisão de 80x60 blocos. Isso permite que o background seja configurado de diferentes formas de acordo com o preenchimento da memória (Fig. 11). Se um endereço for preenchido com o valor 0b111111110 = 510, o Módulo de Desenho entenderá que o bloco correspondente está desabilitado, assim ocupando os pixels da área com a cor base do background, um polígono ou sprite, caso suas coordenadas coincidam com o bloco.

   **Figura 11:** Divisão da área do Background.

4. **Definição de um Polígono (DP):**
   Essa instrução é utilizada para modificar o conteúdo da Memória de Instrução do Co-Processador (Fig. 6), de forma a definir os dados referentes a um polígono que deve ser renderizado. O valor do opcode é configurado como 0011. O campo endereço é utilizado para a escolha da posição de memória em que a instrução será armazenada, possibilitando o controle da sobreposição dos polígonos. Os campos ref point X e ref point Y são usados para definir as coordenadas do ponto de referência do polígono. O campo tamanho define a dimensão do polígono. Caso seu valor seja configurado como 0b0000, o polígono que foi definido estará desabilitado. Por último, as componentes RGB definem a cor do polígono, e o bit de forma define se o polígono corresponde a um quadrado (0) ou triângulo (1). Logo abaixo, na Tabela II, temos as dimensões que atualmente são possíveis de utilização com o Co-Processador.

### Protocolo de Comunicação entre o HPS e o Processador Gráfico

Para enviar instruções ao Processador Gráfico, foi desenvolvido um protocolo de comunicação simples utilizando as FIFOs e o módulo Gerador de Pulso, conforme ilustrado na Fig. 3. Como os jogos serão desenvolvidos em linguagem C, é possível acessar as GPIOs (General-Purpose Input/Output) do sistema através da técnica de Mapeamento de Memória. Usando a ferramenta Platform Design do software Quartus Prime Lite, realiza-se a associação de endereços de memória às entradas e saídas do sistema. Dessa forma, obtém-se acesso a todos os sinais e barramentos conectados ao processador Nios II, conforme mostrado na Fig. 3.
Controle de Acesso e Envio de Instruções

O controle de acesso para escrita e leitura é realizado pelo controlador de barramento do Nios II. Com esses endereços e utilizando as Instruções Customizadas do Nios II, a distribuição dos campos das instruções do Processador Gráfico é realizada dentro dos barramentos dataA e dataB no momento do envio. Como existem instruções com mais de 32 bits, decidiu-se manter dois barramentos de 32 bits separados, permitindo linhas de transmissão suficientes para a implementação de novas instruções de até 64 bits. As Fig. 13, 14, 15 e 16 mostram a distribuição dos dados. O barramento dataA é utilizado para opcodes e endereçamento do Banco de Registrador e Memórias; o barramento dataB é utilizado para envio de dados a serem armazenados e/ou atualizados.

Após a inserção da instrução nos barramentos dataA e dataB, deve-se colocar o sinal start em nível lógico alto. Isso ativará o Módulo Gerador de Pulso, que habilitará a escrita nas FIFOs durante um único pulso de clock, garantindo que as instruções sejam escritas somente uma vez, evitando instruções duplicadas e desperdício de espaço. Em seguida, o sinal start deve ser colocado novamente em nível lógico baixo para reiniciar o módulo Gerador de Pulso, aguardando a próxima mudança na sua entrada para gerar um novo pulso de escrita. Não é possível utilizar um pulso de escrita direto do Nios II, pois as Instruções Customizadas levam cerca de 6 pulsos de clock para refletir o novo valor na respectiva saída, o que resultaria em várias escritas nas FIFOs, causando redundâncias de instruções.

## Módulo de Kernel

O processador de propósito geral acessa a FPGA usando as pontes HPS-to-FPGA ou Lightweight HPS-to-FPGA. Essas pontes são mapeadas para regiões no espaço de memória do HPS. A fim de realizar a conexão do Processador Gráfico na FPGA com o Processador no HPS de forma mais facilitada, sem a necessidade de realizar o mapeamento de memória a cada acesso ao GPU, foi desenvolvido um módulo de kernel. Um módulo de kernel é um código que pode ser carregado e descarregado no kernel do sistema operacional Linux sem precisar recompilar ou reiniciar o sistema. Ele atua como uma extensão do kernel, adicionando novas funcionalidades ou suporte a novos dispositivos de hardware.

No Linux, existe uma grande diferença entre o espaço do Kernel e o espaço do usuário. O espaço do Kernel é uma área de memória reservada para a execução do núcleo do sistema operacional e seus módulos, onde o kernel tem controle total e pode acessar diretamente o hardware. Este espaço é privilegiado e seguro, prevenindo que aplicações de usuário possam interferir no funcionamento do sistema operacional. Por outro lado, o espaço do usuário é destinado à execução de aplicações e programas, sendo uma área menos privilegiada que não permite acesso direto ao hardware ou à memória do kernel, garantindo a estabilidade e segurança do sistema ao isolar as aplicações umas das outras e do núcleo do sistema.

Para conectar esses dois espaços, é possível gerar um arquivo (exclusivo para o hardware a ser acessado) que pode ser lido e/ou escrito, através de um driver de carácter, como mostrado no esquema da figura a seguir:

<img width="" src="">

O objetivo principal do módulo de kernel desenvolvido é fornecer uma interface eficiente para escrita e leitura em dois barramentos, DATA A e DATA B, que conectam o processador HPS da placa DE1-SoC ao processador gráfico, facilitando a comunicação e melhorando o desempenho do sistema.

### Desenvolvimento do Módulo de Kernel

Para o devido funcionamento do módulo de kernel, alguns passos foram seguidos em seu desenvolvimento.

1. Definição da Interface:

Para iniciar foram definidas as bibliotecas a serem importadas para o funcionamento completo do código. Algumas bibilotecas da linguagem C não podem ser utilizadas em códigos do espaço do kernel, então nesses casos, foram substituídas por bibliotecas válidas, como a asm/uaccess.h para acesso a memória e linux/delay.h para a função de sleep. Após a importação das bibliotecas, foram realizadas as definições para os comandos de controle de E/S (ioctl) que serão utilizados para ler e escrever nos barramentos DATA_A e DATA_B. Cada barramento pode receber até 32 bits de dados, totalizando 64 bits para ambos os barramentos. Foram também definidos os locais de memória dos registradores WRREG e WRFULL, utilizados para dar start no envio de uma instrução e verificar se o buffer de instruções está cheio, respectivamente. 

2. Estrutura do Módulo

O módulo de kernel foi estruturado em um arquivo principal (data_bus.c), contendo funções de inicialização, finalização, e operações de leitura e escrita. As principais funções do módulo incluem:

- Inicialização e Finalização: A funções (databus_init e databus_exit) são responsáveis por registrar e liberar o dispositivo de caractere associado ao driver.
- Operações de Controle de E/S (ioctl): As funçãos dev_ioctl lida com os comandos ioctl para leitura e escrita nos barramentos DATA_A e DATA_B.
- Operações de Controle do Arquivo: funções padrão do driver de caractere (read, write, open, release).

3. Drive de Caracteres

O driver de caracter é uma parte fundamental do módulo de kernel quando se trata de comunicação com dispositivos de hardware. Ele fornece uma interface para leitura e escrita de dados entre o software e o hardware. No contexto do módulo de kernel desenvolvido, o driver de caracter desempenha um papel crucial.

- Estrutura e Funcionamento do Driver de Caracter:
    - Registro do Driver: O driver de caracter é registrado no sistema com uma identificação única (major number) que permite ao sistema operacional diferenciar entre diversos drivers de caracter.
    - Funções Principais: O driver deve implementar funções essenciais, como open, release, read e write.
        Open: Função chamada quando um processo tenta abrir o dispositivo. Pode ser usada para inicializar variáveis ou recursos necessários.
        Release: Função chamada quando um processo fecha o dispositivo. Usada para liberar recursos alocados.
        Read: Função chamada para ler dados do dispositivo. Implementa a lógica de leitura de dados do hardware para o espaço de usuário.
        Write: Função chamada para escrever dados no dispositivo. Implementa a lógica de escrita de dados do espaço de usuário para o hardware.
    - Interface com o Hardware: O driver de caracter comunica-se diretamente com o hardware, utilizando registradores de memória mapeados ou portas de E/S, para realizar operações de leitura e escrita.
    - Criação de Arquivo de Dispositivo: Um arquivo especial de dispositivo é criado no diretório /dev, que permite aos usuários e programas acessar o driver de caracter através de operações de arquivo padrão (open, read, write, close).

4. Implementação das Funções de Leitura e Escrita

- Função de Escrita: A instrução que será enviada para os barramentos Data A e Data B é recebida pelo módulo como uma instrução inteira de 64 bits. Logo, é necessário separá-la para escrever separadamente em cada barramento. Para realizar essa divisão, os primeiros 32 bits da instrução recebida são colocados na variável do Data A, e os outros 32 bits são deslocados para a variável do Data B. Isso é feito utilizando operações de máscara e deslocamento de bits, de forma que cada parte da instrução seja corretamente atribuída ao seu respectivo barramento. Exemplo do código a baixo:

  ```c
      uint64_t data;
      uint32_t data_a;
      uint32_t data_b;


      if (copy_from_user(&data, buffer, sizeof(data))) {
        return -EFAULT;
      }

      data_a = (uint32_t)(data & 0xFFFFFFFF);
      data_b = (uint32_t)(data >> 32);
    
      iowrite32(data_a, data_a_mem);
      iowrite32(data_b, data_b_mem);
    
      return sizeof(data);

    ```
  
- Função de Leitura: A função de leitura tem como objetivo coletar os dados dos barramentos de saída do hardware e enviá-los para o espaço do usuário. Para isso, os dados são lidos de dois registradores diferentes e combinados em uma única instrução de 64 bits. Primeiramente, são lidos os 32 bits do primeiro registrador e armazenados na metade inferior da instrução. Em seguida, são lidos os 32 bits do segundo registrador e armazenados na metade superior da instrução. A instrução combinada é então copiada para o buffer de saída do usuário.  Exemplo do código a baixo:

  ```c
      uint32_t data_a;
      uint32_t data_b;
      uint64_t data;
    
      data_a = ioread32(data_a_mem);
      data_b = ioread32(data_b_mem);
    
      data = ((uint64_t)data_b << 32) | data_a;
    
      if (copy_to_user(buffer, &data, sizeof(data))) {
        return -EFAULT;
      }
    
      return sizeof(data);

    ```
As funções copy_from_user e copy_to_user são funções do espaço do kernel utilizadas para realizar a escrita e leitura no hardware.

## Desenvolvimento da Biblioteca

Para estabelecer a comunicação entre a biblioteca e o módulo de Kernel, fazemos uso das instruções `write_data()`, `read_data` e `close_data`.

A função `write_data()`, como o nome pressupõe, efetua a escrita no arquivo utilizado para estabelecer a comunicação entre o modulo do kernel e os barramentos da GPU. As próximas instruções, `read_data()` e `close_data()`, reespectivamente, leem e encerram esse mesmo arquivo.

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
mv GraphicsProcessor-HPS-Kernel-Library/[caminho do destino]
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
