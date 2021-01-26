tests = [ {'description': 'PINA: 0x00 => PORTB: 0x00 ,PORTC: 0x00',
    'steps': [ {'inputs': [('PINA',0x00)], 'iterations': 5 }], 
'expected': [('PORTB',0x00)],
'expected': [('PORTC',0x00)],
},

{'description': 'PINA: 0xFF => PORTB: 0x00 ,PORTC: 0x00',
    'steps': [ {'inputs': [('PINA',0xFF)], 'iterations': 5 }],
'expected': [('PORTB',0x00)],
'expected': [('PORTC',0xF0)],
},

{'description': 'PINA: 0x34 => PORTB: 0x00 ,PORTC: 0x00',
    'steps': [ {'inputs': [('PINA',0x00)], 'iterations': 5 }],
'expected': [('PORTB',0x00)],
'expected': [('PORTC',0x00)],
},



]
#watch = ['PORTB', 'PORTC']
