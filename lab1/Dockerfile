FROM tekky03/alpine:musl as builder
# FROM gcc:latest as builder

WORKDIR /root/

COPY main.c .

RUN cc -o main main.c

# FROM ubuntu:18.04
FROM alpine:latest

WORKDIR /root/

COPY --from=builder /root/main .

CMD ["./main"]
