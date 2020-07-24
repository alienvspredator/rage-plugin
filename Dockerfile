FROM gcc:10 as builder

WORKDIR /project

RUN apt-get update && \
	apt-get install -y \
	libboost-dev libboost-program-options-dev \
	libgtest-dev \
	cmake

COPY . .

RUN cmake . -Bbuild && \
	cmake --build build

CMD [ "cp", "-r", "build", "/out" ]
