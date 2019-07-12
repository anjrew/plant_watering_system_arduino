/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * Additions Copyright 2016 Espressif Systems (Shanghai) PTE LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

/**
 * @file aws_iot_certifcates.c
 * @brief File to store the AWS certificates in the form of arrays
 */

#ifdef __cplusplus
extern "C" {
#endif

const char aws_root_ca_pem[] = {"-----BEGIN CERTIFICATE-----\n
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n
rqXRfboQnoZsG4q5WTP468SQvvG5\n
-----END CERTIFICATE-----\n"};



const char certificate_pem_crt[] = {"-----BEGIN CERTIFICATE-----\n
MIIDWTCCAkGgAwIBAgIUE+N0ZBfjXdEvsgwYLFL7zZb21WYwDQYJKoZIhvcNAQEL\n
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDcwNDEzNTQw\n
MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMkDUFNOMUWGg5a8osCp\n
6fAMd1orKOZr5w2PBbdqa2IRDOgIpdmHqzkuofO0c35FLYOvegnEyQY3W+MJj1H0\n
0t2ClMvYdmSiGQLoSgcess4hk+d79cVAposXXKhjvg6YsTFfQATx88sHFgS0CDZV\n
kaDIp1wXDNlQr/HwudY0zsynJ6jmzEEUAeiSLyL3FwoeV5Q7RfJjPCvTtxcH7KpZ\n
He1KBlsikC7Lbm7rp+ArQwznhvYcd3iprPj9SgMtQmsBmm2SF2BhpLF68MuM3Xlg\n
3ljdgAgdM7pfPj3HVM8vACA+4WTvuLOabMY5jA/PnDQRSpqRF5CVq0WNEi91Kui7\n
4ekCAwEAAaNgMF4wHwYDVR0jBBgwFoAUFP56SjXMFbaqsfr47HorSLju3Q4wHQYD\n
VR0OBBYEFJ+Y/Wma3ZzHAU2je6vhaD0Voy95MAwGA1UdEwEB/wQCMAAwDgYDVR0P\n
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBIzHO1P8/wxm8Nie7o1JXEuS7j\n
lgv2GohxGN11qxb0YR7gMidrCtISMoqGypGbxJhoreWqOd0V86Xnfc1PLJEVfq+D\n
fx1B4i49adolFr5K8iwIf1kyvOzPyibHY3u+FQoY3xAr4+svuCUbw28rXXP6ktpV\n
8SlOxGljqVmGujC+uDiWh5AXeX25em4q5Jl6dNP1B2QcvuUeE8+8PX9OE//ROzz/\n
gK2QSqyhOZdrGh9PyUMbS8uq9Bt/D7NqIWcxEIqPmkxtoDtikdVUFeFlkZZgGAVX\n
imf6PIXrMtJRHxjLt4GqShkQCkm/vH8aU2bA8Zr+rbkmCkE4K+dD2/pEGK21\n
-----END CERTIFICATE-----\n"};

const char private_pem_key[] = {"-----BEGIN RSA PRIVATE KEY-----\n
MIIEpQIBAAKCAQEAyQNQU04xRYaDlryiwKnp8Ax3Wiso5mvnDY8Ft2prYhEM6Ail\n
2YerOS6h87RzfkUtg696CcTJBjdb4wmPUfTS3YKUy9h2ZKIZAuhKBx6yziGT53v1\n
xUCmixdcqGO+DpixMV9ABPHzywcWBLQINlWRoMinXBcM2VCv8fC51jTOzKcnqObM\n
QRQB6JIvIvcXCh5XlDtF8mM8K9O3Fwfsqlkd7UoGWyKQLstubuun4CtDDOeG9hx3\n
eKms+P1KAy1CawGabZIXYGGksXrwy4zdeWDeWN2ACB0zul8+PcdUzy8AID7hZO+4\n
s5psxjmMD8+cNBFKmpEXkJWrRY0SL3Uq6Lvh6QIDAQABAoIBAQC70WfVXsM09jmv\n
t3PO1zRpggxm6NrfacTK627o8PuC9vF7QNFOTrCSzlqublZXjmW3nFdIF2HJeCJY\n
Ic+TQVFhfahwS0JcRwVdrRO2UV26sq8S5MVAWGcEoYCHKiMkWlF+E0SrGjOz6VwO\n
avUdZ5iZhcKyjisIRpcPVeEeH9t2ZbMpBowutgFYbgwi96y8GgJBaE0csJpjDcGe\n
68gawuns4wSoGm+ebGmrTQAIBft5DFLA4Lif2tt378wB71pI4fGI2+NhLBgLQ80C\n
OXT39/21tNuBeFnv5IXkW1cIMD9+ww1EtzwU/R+z1mDNmi0JQ8AkT5z5qHOw8sNi\n
3ud5zSlNAoGBAPkPJkKENZdVvIzdgeMQ8MNwRBbtt/NBpAOoCA2Awk0hxXxBxig2\n
tpVOJnuCN5dFwkQMoyo5uOnY3sMJvW0R3xQFSnH3BUqdYhCdKEZejJHWfAS9TaFI\n
BADmY64FXCmUsxZrvzo7WoboAmb3M4H49DugJ9wGqlchY1TzJy4TwwGrAoGBAM6d\n
Y/UabzQNZnCOLR4vVsgFKze1s98ZxP33ini4+c43ZQLbC1hb9u5olhGk98hcU9ak\n
Prqt7w4fVQncuQ5TJsVSf0F0/C0H5yifcXUW66t7StJglHPH5wT3XR2XRTmnqI4s\n
+P0kNcLfTEAKT+mJlaWMBd7lKeCpoaiktLh65v67AoGBALoddgvS3HFgomvpw3w5\n
VpHVvSwdLI9diAEregkNJSWajQdT5455hGuCYQN+xagHeyp6/xs401P8ukqpZqKw\n
7jD52y3a2FtvzK55onUjhowc6cRo6jARrtw1TlvmqX5xHnssvVP4sqeJjPIA3L7N\n
mUVNYzvqxzQhQSdkJO0r21fNAoGAN/wRv7Jf+3y+CzWbKBivrRcpdLj5ejRjWmWe\n
0cNdfPEpn7J2yoRwznzsXO+UbGgIF5r9nFvQ8MSwqDF0u1rwoHsQHE50nkcL7cf2\n
WEHTre2EgqjE0PKTuJbGVmRMz6iMVan99anAH4OeB6RDrwYm1GqylIkH5eZQ9X9b\n
sLksqk0CgYEAvy0t60jikckQX+lqUChkTr+ItrTe9zlVB7AvJx7xa8iqsMbrLUFx\n
y3XzbSfxFbBeljnlCc+I/k/BtXjwaBi87+pOYiR/CBk0RUE+tMuuOBUEH+OzD4VG\n
1zytFNcJ/VVtxaFt+NCLCvIZwFYmYlnAKNZmfNiRBnIfqo3m04jtzyQ=\n
-----END RSA PRIVATE KEY-----\n"};


#ifdef __cplusplus
}
#endif
