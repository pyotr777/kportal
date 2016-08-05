#!/bin/bash
# Reconfigure Apache for LetsEncrypt SSL certificates

if [[ -f "/ENV" ]]; then
	source /ENV
fi

# Create softlink /etc/letsencrypt -> /etc/kportal/ssl/letsencrypt
cd /etc
ln -s /etc/kportal/ssl/letsencrypt/ letsencrypt

CERT=$(ls /etc/kportal/ssl/letsencrypt/live/$KP_WEB_DNS/cert*.pem | sed -n 1p)
if [[ $? -ne 0 ]]; then
    echo "Certificate file not found: $CERT"
    exit 1
fi
KEY=$(ls /etc/kportal/ssl/letsencrypt/live/$KP_WEB_DNS/privkey*.pem | sed -n 1p)
if [[ $? -ne 0 ]]; then
    echo "Key file not found: $KEY"
    exit 1
fi
CHAIN=$(ls letsencrypt/live/$KP_WEB_DNS/fullchain*.pem | sed -n 1p)
if [[ $? -ne 0 ]]; then
    echo "Chain file not found: $CHAIN"
fi

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