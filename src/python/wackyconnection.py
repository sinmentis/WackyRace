"""Base class for Wacky connections.

Handles command logic and provides a REPL for subclasses to use."""

from __future__ import print_function

__author__ = 'Matthew Edwards'
__date__ = 'January 2015'

import sys


class WackyCommunicationError(EnvironmentError):
    pass


class WackyConnection(object):

    def _readline(self):
        """Read a line from the device, or time out."""
        raise NotImplementedError()

    def _write(self, data):
        """Write the given data to the device."""
        raise NotImplementedError()

    def send_command(self, command):

        prompt = '> \n'

        if self.debug:
            print('Sending command: %s' % command, file=sys.stderr)

        command += '\n'
        self._write(command)

        response = ''
        while True:
            line = self._readline().replace('\r', '')
            if line != '':
                response += line
                if response[-len(prompt):] == prompt:
                    break

        response_lines = [response + '\n' for response in response.split('\n') if response != '']

        if self.debug:
            print('Read response: %s' % response_lines, file=sys.stderr)
            
        # First line should be echoed command, last line should be prompt
        if len(response_lines) == 0:
            raise WackyCommunicationError('No response to command')
        if response_lines[0] != command:
            raise WackyCommunicationError(
                'Command not echoed: expected %r, received %r instead' % (command, response_lines)
            )
        if response_lines[-1] != prompt:
            raise WackyCommunicationError(
                'Expected prompt: received %r instead' % response_lines[-1]
            )

        result = response_lines[1:-1]
        if len(result) == 0:
            # No result returned
            return ''
        elif len(result) == 1:
            # One line returned
            return result[0]
        else:
            # Multiple lines returned
            return ''.join(result)
