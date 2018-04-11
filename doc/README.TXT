O projeto foi desenvolvido e testado no Visual Studio 2017, pelo que é recomendável o seu uso.
Para correr, criar um novo projeto no Visual Studio 2017, e importar os source files (prepFiles não é necessário, é um programa
à parte para criar os ficheiros de entrada). Depois, colocar o GraphViewerController no mesmo diretório do executável: geralmente é na pasta nomeProj/nomeProj/
Abrir as propriedades do projeto, e:
  - em General/ Character Set - > colocar not set
  - em Linker/Input -> Additional Dependencies ... adicionar wsock32.lib.

Para compilar e correr, basta premir CTRL+F5

