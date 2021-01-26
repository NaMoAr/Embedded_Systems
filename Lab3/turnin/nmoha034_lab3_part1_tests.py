tests = [ {'description': 'PINA: 0x00, PINB:0x00  => PORTC: 0x00',
    'steps': [ {'inputs': [('PINA',0x00)], 'iterations': 5 }, 
               {'inputs': [('PINB',0x00)], 'iterations': 5 },
               
'expected': [('PORTC',0x00)],
},
 {'description': 'PINA: 0xF0, PINB:0x0F  => PORTC: 0x08',
    'steps': [ {'inputs': [('PINA',0xF0)], 'iterations': 5 },
               {'inputs': [('PINB',0x0F)], 'iterations': 5 },
               
'expected': [('PORTC',0x08)],
},
{'description': 'PINA: 0xFF, PINB:0xFF  => PORTC: 0x10',
    'steps': [ {'inputs': [('PINA',0xFF)], 'iterations': 5 },
               {'inputs': [('PINB',0xFF)], 'iterations': 5 },
               
'expected': [('PORTC',0x10)],
},

 
]
#watch = ['PORTC']
