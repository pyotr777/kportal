#!/bin/bash
# Reconfigure Apache for LetsEncrypt SSL certificates

if [[ -f "/ENV" ]]; then
	source /ENV
fi

CERT=/etc/kportal/ssl/letsencrypt/live/$KP_WEB_DNS/cert.pem
KEY=/etc/kportal/ssl/letsencrypt/live/$KP_WEB_DNS/privkey.pem

if [[ -f "$CERT" ]]; then
    echo "Certificate file $CERT found."
    # Replace Apache configs
    sed -r -i "s|^\(\s*\)SSLCertificateFile\s*.*|\1SSLCertificateFile $CERT|g" /etc/apache2/sites-available/default-ssl.conf
fi
if [[ -f "$KEY" ]]; then
    echo "Certificate key file $KEY found."
    # Replace Apache configs
    sed -r -i "s|^\(\s*\)SSLCertificateKeyFile\s*.*|\1SSLCertificateKeyFile $KEY|g" /etc/apache2/sites-available/default-ssl.conf
fi