          INP
          SPUSH
          CALL PRINTDOWN
          HLT
PRINTDOWN SDUP
          SPOP
          OUT
          SPUSHI 1
          SSUB
          SDUP
          SPOP
          BRZ DONE
          CALL PRINTDOWN
DONE      RET
