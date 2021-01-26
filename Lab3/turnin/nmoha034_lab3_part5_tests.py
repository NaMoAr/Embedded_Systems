tests = [ {'description': 'PINB: 0x00, PIND: 0x00 => PORTB: 0x00',
    'steps': [ {'inputs': [('PINB',0x00)], 'iterations': 5 }], 
    'steps': [ {'inputs': [('PIND',0x00)], 'iterations': 5 }],
'expected': [('PORTB',0x00)],

},

{'description': 'PINB: 0xFF, PIND: 0xFF => PORTB: 0x02',
    'steps': [ {'inputs': [('PINB',0xFF)], 'iterations': 5 }],
    'steps': [ {'inputs': [('PIND',0xFF)], 'iterations': 5 }],
'expected': [('PORTB',0x02)],

},

{'description': 'PINB: 0x00, PIND: 0x80 => PORTB: 0x02',
    'steps': [ {'inputs': [('PINB',0x00)], 'iterations': 5 }],
    'steps': [ {'inputs': [('PIND',0x80)], 'iterations': 5 }],
'expected': [('PORTB',0x02)],

},



]
#watch = ['PORTB']
