# Mock Epson Hardware & Protocol Simulator for Unit Testing
import sys, os

class MockEpsonPrinter:
    def __init__(self, model="L3250", main_counter=4500, max_counter=6346):
        self.model = model
        self.main_counter = main_counter
        self.max_counter = max_counter
        self.power_on = True

    def query_device_id(self):
        return f"MFG:EPSON;CMD:ESCPL2,BDC,D4,D4PX,ESCPR7;MDL:{self.model} Series;CLS:PRINTER;DES:EPSON {self.model} Series;"

    def handle_bdc_packet(self, packet_bytes):
        # Decode BDC query
        text = packet_bytes.decode('latin1', errors='ignore')
        if "EE:" in text:
            # Format: EE:003A00000000; or EE:003A04;
            if "00000000" in text:
                # Reset command received
                self.main_counter = 0
                return b"\x1b\x01@BDC PS\r\nOK;\x0c"
            else:
                # Read query: return 4 bytes of counter in hex
                c_bytes = self.main_counter.to_bytes(4, byteorder='little')
                hex_str = c_bytes.hex().upper()
                return f"\x1b\x01@BDC PS\r\nEE:003A{hex_str};\x0c".encode('latin1')
        return b"\x1b\x01@BDC PS\r\nOK;\x0c"

if __name__ == '__main__':
    p = MockEpsonPrinter()
    print("Mock Device ID:", p.query_device_id())
    print("Mock EEPROM read:", p.handle_bdc_packet(b"\x1b\x01@BDC PS\r\nEE:003A04;\x0c"))
    print("Mock EEPROM reset:", p.handle_bdc_packet(b"\x1b\x01@BDC PS\r\nEE:003A00000000;\x0c"))
    print("New counter:", p.main_counter)
