# Задание 6.1 (1 балл) 
Выполнить захват пакетов (wireshark), отправляемых клиентом и сервером из задачи 5.1.

Проанализировать структуру и содержание пакетов. На примере конкретных сообщений клиента и сервера указать содержимое полей в заголовках Ethernet, IP и UDP.
## Анализ пакета, полученного сервером
Был получен следующий пакет:
| | |
| - | - |
| 0000 | 00 00 00 00 00 00 00 00 00 00 00 00 08 00 45 00 |
| 0010 | 00 2a c1 23 40 00 40 11 7b 9d 7f 00 00 01 7f 00 |
| 0020 | 00 01 d9 9b c7 38 00 16 fe 29 48 65 6c 6c 6f 20 |
| 0030 | 57 6f 72 6c 64 21 0a 00 |

Выполним разбор попорядку:
### Ethernet
| Код | Длина (байт) | Определение | Пояснение |
| - | - | - | - |
| 00 00 00 00 00 00 | 6 | 00:00:00:00:00:00 | MAC destination |
| 00 00 00 00 00 00 | 6 | 00:00:00:00:00:00 | MAC source |
| 08 00 | 2 | IPv4 (0x0800) | 802.1Q tag |
| 45 00 ... |  |  | Payload

Перейдем к разбору Payload
### IPv4
| Код | Длина (байт) | Определение | Пояснение |
| - | - | - | - |
| 45 | 1 | Version: 4 (4 = 0100) and Header Length: 20 bytes (5 = 0101) | Ethertype and length |
| 00 | 1 | Default(0) and Not ECN-capable Transport (0) | Differentiated Services Codepoint and Explicit Congestion Notification |
| 00 2a | 2 | (0x2a = 42) | Total Length |
| c1 23 | 2 | 0xc123 (49443) | Identification |
| 40 00 | 2 | Don't fragment (010 = 0x2) and 0 offset | Flags and Fragment Offset |
| 40 | 1 | (0x40 = 64) TTL | Time to Live |
| 11 | 1 | UDP (17) | Protocol |
| 7b 9d | 2 | validation disabled (0x7b9d) | Header Checksum |
| 7f 00 00 01 | 4 | (7f 00 00 01 -> 127.0.0.1) | Source Address |
| 7f 00 00 01 | 4 | (7f 00 00 01 -> 127.0.0.1) | Destination Address |
### UDP
| Код | Длина (байт) | Определение | Пояснение |
| - | - | - | - |
| d9 9b | 2 | (d9 9b = 55707) | Source Port |
| c7 38 | 2 | (c7 38 = 51000) | Destination Port |
| 00 16 | 2 | (00 16 = 22) | Length |
| fe 29 | 2 | unverified | Checksum |
### Data
| Код | Длина (байт) | Определение | Пояснение |
| - | - | - | - |
| 48 65 6c 6c 6f 20 57 6f 72 6c 64 21 0a 00| 14 | Hello World!** |