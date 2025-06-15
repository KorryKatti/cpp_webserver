FROM alpine:latest

# install deps
RUN apk add --no-cache g++ cmake make boost-dev asio-dev libc-dev

# setup working dir
WORKDIR /app

# copy code
COPY . .

# build
RUN cmake . && make

# expose port
EXPOSE 18080

# run it
CMD ["./server"]
