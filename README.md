# RSA-Co-CP-ABE
A Novel Hybrid Encryption Scheme using RSA-Co-CP-ABE for Ciphertext Transmission in Cloud Environment



## Operating environment:

$$
\begin{array}{|c|c|}
\hline \text { CPU: } & \text { Intel(R) Xeon(R) Gold 6148 CPU @ 2.40GHz } \\
\hline \text { Operation System: } & \text { Ubuntu } 18.04 .4 \\
\hline \text { Random Access Memory: } & 2 \mathrm{G} \\
\hline \text { Programming Languages: } & \mathrm{C++} \\
\hline \text { Development Tools: } & \text { OpenABE } \\
\hline
\end{array}
$$


## Installation

This section describes the installation of the OpenABE source code (`libopenabe-1.0.0-src.tar.gz`) on various platforms. The OpenABE currently supports several operating systems including multiple versions/distros of Linux, Mac OS X and Windows.

### Debian/Ubuntu-based Linux

To compile OpenABE on Ubuntu or Debian Linux-based distro, first run the `deps/install_pkgs.sh` script from the source directory to install the OpenABE system-specific dependencies as follows:
	
	cd libopenabe-1.0.0/
	sudo -E ./deps/install_pkgs.sh

Note that you only have to do this once per system setup. After completion, you can proceed to compile the OpenABE as follows:

	. ./env
	make
	make test

All the unit tests should pass at this point and you can proceed to install the OpenABE in a standard location (`/usr/local`) as follows:

	sudo -E make install

To change the installation path prefix, modify the `INSTALL_PREFIX` variable in `libopenabe-1.0.0/Makefile`.

## Quick Start

To compile example C++ apps that use the high-level OpenABE crypto box API, do the following:

	. ./env
	make examples
	cd examples/

Then, our contribution can then be verified as follows:



First, start the server and listen to the client, waiting for the client to send the RSA public key:

```shell
Server:
./my_server
```

Next, open the client and send the RSA public key encrypted with CP-ABE to the server(At this point you need to reopen a terminal.):

~~~python
Client:
./my_client 8191
~~~

The server receives the key sent by the client and sends confidential information:
	

```shell
Server:
client ip: 127.0.0.1     port: 44096
recive[708]: AAABrKETqm/Js74xM0Ovfc.....QpAgq8Q8wwDVGZRYhYuWCbQ==
The result after CP-ABE decryption is :
8191
e:8191
d:31
Please enter the message to be sent to Client:

# HelloWorld is the message sent.
HelloWorld

Real message sent：eh^h,]
write finished. len: 10
```

The client receives the information:

~~~shell
Client:
real message：
HelloWorld
client finished.
~~~

## Copyright and License

### License

* [`license.txt`](license.txt) ([Apache License 2.0][Apache License 2.0] License)

[Apache License 2.0]: https://opensource.org/license/apache-2-0/ "Apache License, Version 2.0 | Open Source Initiative"

### Quote
This work has been compiled into [paper](https://ieeexplore.ieee.org/abstract/document/10263473/) and published in IEEE. You are welcome to cite this work in the paper.
```bibtex
@inproceedings{RSA-Co-CP-ABE,
  title={A Novel Hybrid Encryption Scheme Using RSA-Co-CP-ABE for Ciphertext Transmission in Cloud Environment},
  author={Chen, Sizhou and Xie, Xiao and Wang, Xinchen},
  booktitle={2023 IEEE International Conference on Sensors, Electronics and Computer Engineering (ICSECE)},
  pages={1690--1697},
  year={2023},
  organization={IEEE}
}
```

