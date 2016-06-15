FROM ubuntu:14.04
MAINTAINER Peter Bryzgalov

RUN apt-get update && apt-get install -y apache2 apache2-doc apache2-utils openssl openssh-server
RUN mkdir -p /etc/kportal/www/ssl/
RUN a2enmod ssl
RUN a2ensite default-ssl
ADD settings/ubuntu /settings/ubuntu
ADD src/AppMarket /etc/kportal/www
RUN echo "Copy configuration files"
# RUN mv /etc/apache2/apache2.conf /etc/apache2/apache2-default.conf
RUN cp /settings/ubuntu/apache2.conf /etc/apache2/apache2.conf
# RUN mv /etc/apache2/ports.conf /etc/apache2/ports-default.conf
RUN cp /settings/ubuntu/ports.conf /etc/apache2/ports.conf
RUN cp /settings/ubuntu/sites-enabled/default-ssl.conf /etc/apache2/sites-available/default-ssl.conf
RUN cp /settings/ubuntu/sites-enabled/000-default.conf /etc/apache2/sites-available/000-default.conf

# RUN echo "Generate SSL certificate and key"
#ENV country="JP"
#ENV state="Chiba"
#ENV locality="Narashino"
#ENV organization="stair.lab"
#ENV commonname="localhost"
#ENV email="kportal.aics.riken@gmail.com"
#RUN openssl req -x509 -nodes -days 1095 -newkey rsa:2048 -out /etc/kportal/www/ssl/server.crt \
# -keyout /etc/kportal/www/ssl/server.key \
# -subj "/C=$country/ST=$state/L=$locality/O=$organization/OU=$organizationalunit/CN=$commonname/emailAddress=$email"
RUN mkdir /var/run/sshd

EXPOSE 80
EXPOSE 443
EXPOSE 22

CMD ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]