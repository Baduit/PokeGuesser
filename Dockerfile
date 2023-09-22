FROM ubuntu:22.10 
WORKDIR /pokeguesser
COPY . /pokeguesser/

ENV DEBIAN_FRONTEND noninteractive
RUN apt update

RUN apt install python3 python3-pip -y
RUN pip install "fastapi[all]" fastapi_utils "uvicorn[standard]" requests pyserde

RUN apt install nodejs npm -y

WORKDIR /pokeguesser/front
RUN npm install
RUN npm run build


WORKDIR /pokeguesser
CMD ["uvicorn", "Guesser:app", "--host", "0.0.0.0", "--port", "1234"]

EXPOSE 1234