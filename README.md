# TCP Server (main)

**TCP Server** is responsible for data transmission, processing, and storage.

- `Protobuf` is used for data exchange and processing.
- `Boost.asio` is used for connecting between the **client** and the **server**.
- `TCP/IP` stack
- `C++` programming language


## Quick start

A quick start that will help you get the server up and running quickly

## Cloning into github

Clones an open-source repository from github

```cmd
git clone https://github.com/parmigiano/parmigiano-tcp.git
```

## Change directory to parmigiano-tcp

After copying the source files, you need to navigate to the folder containing them in order to continue working

```cmd
cd parmigiano-tcp
```

## Create directory build

Creating a `build` folder that will contain different types of builds

```cmd
mkdir build && cd build
```

## Create subdirectory nmake

A subdirectory where the nmake-generated files will be stored

```cmd
mkdir nmake && cd nmake
```

## Generating with nmake

Generating build files for nmake

```cmd
cmake -G "NMake Makefiles" ../.. -DCMAKE_BUILD_TYPE=Release
```

If error occurs when generating files. which reports that PostgreSQL could not be found, you must specify full path to it as shown below

```cmd
cmake -G "NMake Makefiles" ../.. -DPostgreSQL_ROOT="path_to_PostgreSql/version" -DCMAKE_BUILD_TYPE=Release
```

## Build project

Will building all file (libraries and project). Takes up more memory and takes longer to build

```cmd
nmake
```

Clean build. You'll need to rebuild using nmake

```cmd
nmake clean
```
