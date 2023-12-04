FROM debian:latest

RUN apt-get update && apt-get install -y \
	build-essential \
	wget \
	tar \
	make \
	gcc \
	libc6-dev \
	&& rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/ft_ping

COPY . /usr/src/ft_ping

RUN make \
	&& make clean

RUN wget https://ftp.gnu.org/gnu/inetutils/inetutils-2.0.tar.gz \
	&& tar -xzvf inetutils-2.0.tar.gz \
	&& rm inetutils-2.0.tar.gz \
	&& cd inetutils-2.0 \
	&& ./configure \
	&& make

ENV PATH="/usr/src/ft_ping/inetutils-2.0/ping:${PATH}"

ENTRYPOINT ["/bin/bash"]
