#!/bin/bash
# Reconfigure Apache for LetsEncrypt SSL certificates

CERT=/etc/letsencrypt/live/kportal.ml/cert.pem
KEY=/etc/letsencrypt/live/kportal.ml/privkey.pem

if [[ -f "$CERT" ]]; then
    echo "Certificate file $CERT found."
    # Replace Apache configs
    sed -r -i "s|SSLCertificateFile\s*.*|SSLCertificateFile $CERT|g" /etc/apache2/sites-available/default-ssl.conf
fi
if [[ -f "$KEY" ]]; then
    echo "Certificate key file $KEY found."
    # Replace Apache configs
    sed -r -i "s|SSLCertificateKeyFile\s*.*|SSLCertificateKeyFile $KEY|g" /etc/apache2/sites-available/default-ssl.conf
fi