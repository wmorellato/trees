# Algoritmo

Algoritmo tirado de *[Self-organizing tree models for image synthesis](http://algorithmicbotany.org/papers/selforg.sig2009.pdf)*. Usando [esse repositorio](https://github.com/bernardosulzbach/self-organizing-tree-models) como referência.

# Requisitos

- cmake > 3.15
- gcc > 7?
- [conan](https://conan.io/)

# Build

Primeiro muda o caminho do arquivo de configuraçao [src/environment.hpp:10](src/environment.hpp) pro exemplo aqui [config/config.yaml](config/config.yaml).

Agora tem que dar build na biblioteca Easy3D. Vai pra `libs/` e executa `install_easy3d.sh`. Tem que rodar o script da pasta `libs/` porque o CMake vai procurar lá a biblioteca. Foi o único jeito que encontrei de integrar essa biblioteca no build, mas como só vou usar ela pra visualizar as árvores enquanto desenvolvo, depois não vou precisar mais.

Agora pode dar build no projeto.

```bash
cd <project_root>
cmake -B ./build
cd build && cmake --build .
```

O binário vai tá em `build/src/trees`. Roda passando o número de iterações como argumento, e.g. `trees 10`.