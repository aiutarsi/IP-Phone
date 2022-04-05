# IP-Phone

## OverView

Simple server-client model IP-Phone. This program uses C socket programming. 

* Bidirectional telephone call (using multi-thread)

* Recording automatically, and playing it

## Installation

Execute the below command on your terminal or download the Zip file.

```
git clone https://github.com/aiutarsi/IP-Phone.git
```

After that, change the directory to the downloaded directory.

## Usage

### Requirements

* gcc
* sox
* bash

### Compilation

* Server-Side

```
$ gcc -o serv_send3_multi serv_send3_multi.c -pthread
```

* Client-Side

```
$ gcc -o client_recv3_multi client_recv3_multi.c -pthread
```

### Searching for your IP address(IPv4)

Please search for your IP address(IPv4), by using `ifconfig` etc... on both the server and the client-side. This process needs only the server-side, and the server-side must tell the client-side his/her IP address.

### Start Phone

* Server-Side

After giving the executive authority to `server` by like `chmod`, Execute the below command on the terminal. Of course, 50000 is an example.

```
$ ./server 50000(Port Number)
```

* Client-Side

After giving the executive authority to `client` by like `chmod`, Execute the below command on the terminal. Of course, 50000 is an example. Be careful, the IP address is **Server's address**, and 192.68.1.1 is also an example.

```
$ ./client 192.168.1.1(Server's IPv4) 50000(Port Number)
```

### Hang Up

Please type `ctrl + c` from the server or client-side.

### Playing Recording

* Server-Side

After giving the executive authority to `play_log_server` by like `chmod`, Execute the below command on the terminal. The argument is `input` or `output`. `input` is your voice which you speak(server side's voice). `output` is the partner's voice(client side's voice).

```
./play_log_server input 
```

* Client-Side

After giving the executive authority to `play_log_client` by like `chmod`, Execute the below command on the terminal. The argument is `input` or `output`. `input` is your voice which you speak(client side's voice). `output` is the partner's voice(server side's voice).

```
./play_log_client input 
```


## License

```
MIT License

Copyright (c) 2022 aiutarsi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
