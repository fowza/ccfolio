FROM ubuntu:23.10

RUN apt-get update && apt-get install -y libpistache-dev odb

COPY ./build/app/ccfolio-api /app/ccfolio-api

WORKDIR /app

CMD ["./ccfolio-api"]
