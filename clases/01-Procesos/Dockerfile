FROM ubuntu:14.04
RUN apt-get update
RUN apt-get install -y valgrind \
  g++ \
  cmake
COPY main5.cc /root
COPY main6.cc /root
RUN g++ -Wall -g -DEJEMPLO_5 /root/main5.cc -o /root/ejemplo5
RUN g++ -Wall -g -DEJEMPLO_6 /root/main6.cc -o /root/ejemplo6
WORKDIR /root
ENTRYPOINT ["valgrind"]
