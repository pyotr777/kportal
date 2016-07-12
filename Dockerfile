FROM ubuntu:14.04
MAINTAINER Peter Bryzgalov
#
# Build image with Apache2 and SSL.
#
RUN apt-get update && apt-get install -y apache2 apache2-doc apache2-utils openssl openssh-server php5 libapache2-mod-php5
RUN a2enmod ssl
RUN a2ensite default-ssl
ADD settings/ubuntu /settings/ubuntu
RUN echo "Copy configuration files"
RUN cp settings/ubuntu/apache2.conf /etc/apache2/apache2.conf
RUN cp settings/ubuntu/ports.conf /etc/apache2/ports.conf
RUN cp settings/ubuntu/sites-enabled/default-ssl.conf /etc/apache2/sites-available/default-ssl.conf
RUN cp settings/ubuntu/sites-enabled/000-default.conf /etc/apache2/sites-available/000-default.conf

RUN mkdir /var/run/sshd
RUN mkdir /certbot
ADD install_certbot.sh /certbot/


EXPOSE 80
EXPOSE 443
EXPOSE 22

CMD ["/usr/sbin/apache2ctl", "-D", "FOREGROUND"]
