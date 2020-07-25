FROM gcc:10 as builder

RUN apt-get update && \
	apt-get install -y \
	libboost-dev libboost-program-options-dev \
	libgtest-dev \
	cmake

WORKDIR /project

ENTRYPOINT cmake . -B/out && cmake --build /out
