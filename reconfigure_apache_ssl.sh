#!/bin/bash
# Reconfigure Apache for LetsEncrypt SSL certificates

if [[ -f "/ENV" ]]; then
	source /ENV
fi

CERT=/etc/kportal/ssl/letsencrypt/live/$KP_WEB_DNS/cert.pem
KEY=/etc/kportal/ssl/letsencrypt/live/$KP_WEB_DNS/privkey.pem
CHAIN=/etc/kportal/ssl/letsencrypt/live/$KP_WEB_DNS/lets-encrypt-x1-cross-signed.pem

if [[ -f "$CERT" ]]; then
    echo "Certificate file $CERT found."
    # Replace Apache configs
    sed -r -i "s|^(\s*)SSLCertificateFile\s*.*|\1SSLCertificateFile $CERT|g" /etc/apache2/sites-available/default-ssl.conf
fi
if [[ -f "$KEY" ]]; then
    echo "Certificate key file $KEY found."
    # Replace Apache configs
    sed -r -i "s|^(\s*)SSLCertificateKeyFile\s*.*|\1SSLCertificateKeyFile $KEY|g" /etc/apache2/sites-available/default-ssl.conf
fi

if [[ -f "$CHAIN" ]]; then
    echo "Certificate chain file $CHAIN found."
    # Replace Apache configs
    sed -r -i "s|^(\s*)SSLCertificateChainFile\s*.*|\1SSLCertificateChainFile $CHAIN|g" /etc/apache2/sites-available/default-ssl.conf
fi

echo "Set ServerName to $KP_WEB_DNS in /etc/apache2/apache2.conf"
sed -r -i "s|^(\s*)ServerName\s*.*|\1ServerName $KP_WEB_DNS|g" /etc/apache2/apache2.conf