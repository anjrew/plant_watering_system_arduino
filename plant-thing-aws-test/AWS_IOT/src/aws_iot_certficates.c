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
MIIDWTCCAkGgAwIBAgIUORB0wT5dS3jvbXpda03BmqXQvGIwDQYJKoZIhvcNAQEL\n
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDcwNDExMzUw\n
OFoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAO/ny7P2Xt58SWPHNDeH\n
K8fSbqCb9lzJHOtMyxZcOZwhu3PdPeJY75zvds9Z4aACXHGOflj+MJuyihtlqu+U\n
2w0GNLd+CCqxXYo7F2v9HMVj4hNGOfvsD0AncSE8tXJrGqUSxhpsCue0wfeUaNjP\n
NfXPnM55dabnK+biwFKuvNWJRT3B/mpG00m/wtDEWtGINvGOVfnie+8XCdemL0f8\n
OMRaPma3qi30zlD8Zit0ARdrkt2gAAKwblW97NzUsozPDN2kISmnNxgdABf6Dw/A\n
8PgsC6ozUjdMNSOlkFmTDzLzMO7EvRfp0+JpmITphkqeYGxKRdnJuvbUxS9fYprV\n
3wMCAwEAAaNgMF4wHwYDVR0jBBgwFoAUvVLBWObxsX0rs1wmaXXfl1eL7ucwHQYD\n
VR0OBBYEFGkt58hoG9BlbeWb0vm+iCJvTPxcMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAcwOn8LYcW67S6zRQlT7ljNed6\n
adtKaII7CdA2h9rRV+fx4MzIXLoVbdu7EyAdqjKYXIMVgSWK0Q1R2fMtk48dEyN8\n
p+6CLPgFGc0/j0JKL+VvfILV440I9RHfi2759GMdzpyRIsq41L7DhAkDkCeyZwDh\n
w7VTnxmUxOuUoTQNsEZ5zxPSzjKwNETflmbf0sCXqph34t8BkoNyY5megMT3u5Nh\n
WohdO3sAICl7dQFB1e7rqs3tF3uUulylfIrHXr0KVmJqsPMt0VM0wMbXeFC85/lU\n
cNip0lkvarZ2YygkgAXQuQ1FVXy7Xd0oJJJqfP8+9YK7JSFu/WlxJk3MHVPY\n
-----END CERTIFICATE-----\n"};

const char private_pem_key[] = {"-----BEGIN RSA PRIVATE KEY-----\n
MIIEpAIBAAKCAQEA7+fLs/Ze3nxJY8c0N4crx9JuoJv2XMkc60zLFlw5nCG7c909\n
4ljvnO92z1nhoAJccY5+WP4wm7KKG2Wq75TbDQY0t34IKrFdijsXa/0cxWPiE0Y5\n
++wPQCdxITy1cmsapRLGGmwK57TB95Ro2M819c+cznl1pucr5uLAUq681YlFPcH+\n
akbTSb/C0MRa0Yg28Y5V+eJ77xcJ16YvR/w4xFo+ZreqLfTOUPxmK3QBF2uS3aAA\n
ArBuVb3s3NSyjM8M3aQhKac3GB0AF/oPD8Dw+CwLqjNSN0w1I6WQWZMPMvMw7sS9\n
F+nT4mmYhOmGSp5gbEpF2cm69tTFL19imtXfAwIDAQABAoIBABpUrZXl74mpyIBM\n
B94kVtjKrtuv4n1SkXr95bhhjO0nAqT33mfubPYjjj251rqyJ6VLB3ELDRi5otDY\n
+sbq5pPqX3TYP1s4NlUKoUp38GV1zgVhitGsmgvniFImkJ+rQ7RkeBvESfoea7+Z\n
qVxKedit3b8Hs7MK4lWoA4XPqeghv3Dnkzwf72gQxJR+SoOhhqC24SMVLCG9UaX+\n
erRbvfc5GejGjHWxJYvBqwxs6flxtV+yP+vOuLXjT3ckZvZZzVAEsR1PVTkr0FPz\n
R7K8qjs4YkyqzYJH/+xfVnQcvqHWFV03k5cN1SZerC+kArR62py5JFtX+OgbkhNc\n
PZooY2kCgYEA+2QVFkDAo2FLeQjJsMaW3atb3YRf3CXOa05z7ywDHVZqAgGI+koN\n
7C3EAmOOoj6p4aA86udAq3IOwhU8ray4NLKsyRdVUtrfSkOwv8NN6nEoVE60LRJX\n
R8HdjnMNi3fOo+B7M72U3pLEZzYh7YfDQ8JI4E8ZALCv0Sjmx2S9cU8CgYEA9E3O\n
NtzbXv5oPFq40nnXHAzzO2xaQd3yDNqxddsu31GkUUFl4QgccCVP/jQFLIJtGdxG\n
X/OEYpio0cnFAFQfHQFurFA7GEoosb6pWDIQaANPHQBGg8+qJSN0IKuBiZ5KDk7+\n
BWXP/HUgxwY9bGX7NgTyciiy7OFfIvidg75Dgg0CgYEA7L3ltd0pDTOVNdh2yElc\n
zkiP7N66v8HC0P9C4twZ2skEb/suGjcfQWogCvfi3RwSM+8JL1a0akCxcUk6qXvc\n
xZP2cDHPIoYIfhGdJBozCDOkfTDZMRDyrRr/ISVsfglHysCeha1lwu2IlIJAFUcj\n
Nm6C3ureau35xQm1u9U9KrUCgYEAxc3lhKhH+l8UeKGf+dLq52xY2Ck/3wrcO6oq\n
e1lvVyOcXqoL2iseE3MylIj8E3C683BVjsT10Hr4XXv3xO9Xjb2mJWCEqIMK9feA\n
18cZX44mVT3u2jSe95kAIp+ar5l5LqKorCgmQat4UYzs2MKtpqxUvTfUv1UvGXqg\n
1pWLxVUCgYBnc4tTLOhZBwmfHQk9Ci1hH2EiKQ4a9tD85p7600PHOs+FZKis0AzP\n
jQzRRcCkdGPT2/Km4jrOhaT96CFSNTNvDwixgl4TgCVPMBlhDXgCvtuyopSI+Nqg\n
Km2L7W9qKzPZiw/o8N/pU3SR01oOjkeHml+bf6Gslhok210RdXR9fA==\n
-----END RSA PRIVATE KEY-----\n"};


#ifdef __cplusplus
}
#endif
