FROM ubuntu:22.10 
WORKDIR /pokeguesser
COPY . /pokeguesser/

ENV DEBIAN_FRONTEND noninteractive
RUN apt update

RUN apt install python3 -y
RUN apt install python3-pip -y
RUN pip install --no-cache-dir -r requirements.txt

RUN apt install nodejs  -y
RUN apt install npm -y

WORKDIR /pokeguesser/front
RUN npm install
RUN npm run build


WORKDIR /pokeguesser
CMD ["uvicorn", "Guesser:app", "--host", "0.0.0.0", "--port", "1234"]

EXPOSE 1234