# Trabajo Práctico Especial - Arquitectura de las Computadoras ITBA - 20201C

Se adjunta en este archivo la resolución al enunciado denotado por la cátedra para este cuatrimestre. En el mismo se implementó un sistema operativo en Pure64 (a partir de una implementación propuesta) capaz de manejar dos ventanas en simultáneo donde correrán dos procesos principales, un intérprete de comandos y una calculadora.

### Pre-requisitos - Setup del environment

1- Antes que nada, es necesario instalar los siguientes paquetes:

nasm qemu gcc make

2- Tambien se recomienda instalar Docker y utilizar la imagen mencionada en el instructivo de Docker incluido con el proyecto. De esta manera se garantiza que usted compilará el proyecto con las mismas distribuciones de GCC y de Make que el grupo y la cátedra utilizan. Si no sabe como utilizar Docker, el instructivo sera de ayuda.

### Como compilar

1- Situarse dentro de la carpeta del proyecto y ejecute los siguientes comandos
NOTA: si utiliza Docker, siga el instructivo incluido para poder acceder a la carpeta del proyecto desde Docker

```
cd RowDaBoat-x64barebones-d4e1c147f975
cd Toolchain
make all
cd ..
make all
```

### Como ejecutar el proyecto

Ejecute el siguiente comando

```
./run.sh
```

### Dentro del programa

1- Dentro del sistema, se despliega la consulta de cuál va a ser el módulo (programa) a correr en la primer ventana (la de la izquierda), la otra por descarte irá a la segunda ventana (derecha). Para cambiar entre programas se debe utilizar la tecla F3.

2- El funcionamiento del intérprete de comandos se basa en distintas funciones que se pueden invocar que desplegaran cierta información, para acceder a la descripción de ellas es suficiente con escribir “help” y apretar enter.

3- El funcionamiento de la calculadora consta en resolver la expresión escrita, para demarcar que se terminó de escribir se debe apretar la tecla ‘=’. Para saber más sobre el funcionamiento, escriba "help" y luego presione '='

### Teclas especiales:

	-F1: guarda un snapshot de los registros en el momento que se toca esta tecla. Se pueden acceder a estos datos con el comando “inforeg” en el intérprete.
	-F2: borra la línea de entrada hasta el momento.
	-F3: se realiza el cambio de procesos.

## Autores

* Arca, Gonzalo - Legajo: 60303
* Parma, Manuel - Legajo: 60425
* Rodriguez, Manuel Joaquin - Legajo: 60258
