FROM tekky03/alpine:musl as builder

WORKDIR /root/

COPY ./* ./

RUN g++ -o reader reader.cc && g++ -o writer writer.cc && g++ -o main main.cc

FROM alpine:latest

WORKDIR /root/

RUN apk add --no-cache libstdc++

COPY --from=builder /root/* ./
