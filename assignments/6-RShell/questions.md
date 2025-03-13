1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

We use null terminators, newlines, or special characters to indicate when an output is fully sent. Keeping track of string lengths or waiting for the special character is a technique used to ensure complete message transmittion. 

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A protocol should utilize delimeters & str length. Some challenges that can arise is that messages are sent in fragments which can lead to incorrect command parsing. 

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain client session information, while stateless protocols treat each request independently.


4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP offers low latency, and faster transmissions. 

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

Functions like socket(), bind(), listen(), connect(), send(), and recv() are used for both TCP and UDP protocols.