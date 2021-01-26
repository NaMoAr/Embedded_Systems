tests = [ {'description': 'PINA: 0x00  => PORTC: 0x40',
    'steps': [ {'inputs': [('PINA',0x00)], 'iterations': 5 }, 
'expected': [('PORTC',0x40)],
{'description': 'PINA: 0xF3  => PORTC: 0x70',
    'steps': [ {'inputs': [('PINA',0xF3)], 'iterations': 5 },


'expected': [('PORTC',0x70)],

{'description': 'PINA: 0x0A  => PORTC: 0x40',
    'steps': [ {'inputs': [('PINA',0x00A)], 'iterations': 5 },


'expected': [('PORTC',0x3E)],

},
]
#watch = ['PORTC']
