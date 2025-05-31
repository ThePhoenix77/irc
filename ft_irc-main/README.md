# ft_irc Project - IRC Server Implementation

## Overview

This project involves building a custom IRC server, based on the RFC 1459 (and parts of RFC 2812) standards. It will support multiple clients, handle basic IRC commands, manage channels and users, and provide a robust and scalable networking solution. The project is divided into three roles, each focusing on a critical area of the IRC server.

## Summary: bi kholassa
RFC 1459 provides a comprehensive specification for the Internet Relay Chat protocol, outlining its fundamental architecture, communication mechanisms, and 
operational rules. The protocol, designed in the early days of the internet, reflects the technological landscape and priorities of that era. While it has 
served as a foundational technology for real-time text-based communication, its acknowledged limitations, particularly in terms of scalability and 
consistency, have paved the way for the development of more modern communication protocols. Nevertheless, RFC 1459 remains a significant document in the 
history of internet technologies, offering valuable insights into the design principles and challenges of early distributed communication systems. The 
continued existence of IRC networks and the availability of software implementations demonstrate its enduring legacy and ongoing relevance in certain online 
communities.

## Technical Overview of RFC 1459(Internet Relay Chat Protocol aka "IRC")

1. Executive Summary:

This report provides a high-level overview of the Internet Relay Chat (IRC) protocol as defined in RFC 1459. The primary purpose of this Request for Comments document is to establish a standardized technical specification outlining the fundamental concepts and operational rules governing IRC. IRC facilitates real-time, text-based communication through a network of interconnected servers, enabling users to interact via channels or through direct private messaging. The core of the IRC network comprises servers that form a spanning tree topology, and clients that connect to these servers to participate in conversations. Communication within the IRC network relies on a message-based protocol where clients and servers exchange commands and responses in a structured format. The early publication date of this RFC, in May 1993 , indicates that it documents the foundational specifications of a relatively early internet communication technology. This historical context is essential for understanding the design choices and inherent limitations of the protocol when compared to more contemporary communication systems. The existence of this RFC underscores the necessity for standardization in ensuring different IRC clients and server implementations can interoperate seamlessly, highlighting the critical role of open standards in the development and widespread adoption of internet technologies.  

2. Core Components of IRC:

2.1. Servers:

At the heart of the IRC network are servers, which act as central hubs facilitating connections between clients and other servers. These servers are interconnected in a spanning tree topology. This specific network arrangement, while effective in preventing network loops, inherently introduces certain constraints. For instance, the distance between any two servers, and consequently the path messages might take, can be significant, potentially affecting communication latency. Furthermore, the spanning tree structure might present single points of failure, where the malfunction of a critical server could disrupt communication across a segment of the network. This design choice likely reflected the prevailing network infrastructure and understanding of network design principles at the time of the protocol's inception.  

2.2. Clients:

Entities other than servers that connect to an IRC server are defined as clients. A fundamental requirement of the protocol is that each client must be identified by a unique nickname within the network. To maintain network awareness and potentially for administrative purposes, IRC servers are mandated to keep track of specific information for each connected client. This includes the real name of the host machine the client is using, the username of the client on that host, and the specific server to which the client is directly connected. This requirement for servers to maintain detailed client information suggests a design philosophy that placed importance on user identification and potentially network accountability. Such tracking capabilities could have implications for user privacy and the overall security model of the network.  

2.3. Operators:

Within the IRC ecosystem, a special class of clients known as operators exists, possessing elevated privileges beyond regular users. These operators are granted the authority to perform crucial network maintenance tasks. Their responsibilities include the ability to disconnect and reconnect servers, thereby managing the network's physical connectivity. Additionally, operators have the power to forcibly remove users from the network using the controversial "KILL" command. The inclusion of operators with such powerful commands underscores the inherent need for administrative oversight and moderation within a distributed communication system like IRC. The acknowledgment of the contentious nature of the "KILL" command suggests potential concerns regarding its use and the impact it could have on users' experience and network stability.  

2.4. Channels:

IRC facilitates group communication through the concept of channels. These are named virtual spaces that multiple clients can join to participate in discussions and receive messages addressed to that specific channel. A channel is dynamically created when the first user joins it and automatically ceases to exist when the last user departs. The protocol also defines naming conventions for channels and distinguishes between different types, namely distributed channels, which are accessible across the entire network, and local channels, which might have a more restricted scope. The introduction of channels as the primary mechanism for group interaction is a cornerstone of IRC's design, enabling the formation of communities and topic-based discussions. The distinction between distributed and local channels hints at potential variations in how channel scope and management are handled within different parts of the IRC network.  

2.5. Channel Operators:

Furthering the concept of decentralized management, IRC introduces the role of channel operators, often referred to as "chops" or "chanops". These are users who are granted control over specific channels they operate. Their responsibilities include managing the channel's membership by kicking users out, modifying channel settings through modes, inviting new users to join, and setting or changing the topic of discussion for the channel. The delegation of administrative responsibilities to channel operators allows for a distributed model of community management within the broader IRC network. This approach empowers users to govern their own spaces, fostering more dynamic and self-regulating communities.  

3. Protocol Specification:

3.1. Overview:

The IRC protocol serves as the communication language for both server-to-server interactions and client-to-server interactions. While the protocol governs both types of connections, it imposes stricter rules and expectations for client connections compared to those between servers. This difference in stringency likely reflects the differing roles and trust levels associated with clients and servers within the network. Server-to-server communication would inherently require more complex mechanisms for network state synchronization and management.  

3.2. Character Codes:

The underlying structure of the IRC protocol is based on 8-bit octets. However, the specification does not mandate the use of a specific character encoding. Instead, it emphasizes that the delimiters and keywords used within the protocol are designed to be largely compatible with USASCII terminals. This design choice likely reflects the dominant character encoding standards prevalent at the time of IRC's development. Notably, the RFC acknowledges the Scandinavian origins of IRC and mentions the equivalence of certain characters relevant to those languages , suggesting an early, albeit limited, consideration for international character sets within the constraints of the existing technological landscape.  

3.3. Messages:

A critical aspect of the IRC protocol is the defined structure of messages exchanged between clients and servers.  

3.3.1. Message Format:

An IRC message is composed of several distinct parts: an optional prefix, a command, and up to fifteen command parameters. These components are separated by spaces, and each complete message is terminated by a carriage return and line feed (CR-LF) pair. The prefix, when present, is indicated by a leading colon and is primarily used by servers to denote the origin of the message. Clients are generally advised not to use prefixes, except in the specific case of their registered nickname. The command itself can be either a valid IRC command, such as JOIN or PRIVMSG, or a three-digit numeric reply code indicating the outcome of a previous command. The protocol imposes a maximum length of 512 characters for any IRC message, including the terminating CR-LF. This fixed message format, coupled with the length limitation, suggests a design aimed at simplicity and efficiency, likely influenced by the network bandwidth and processing capabilities available in the early 1990s. The clear separation of message components facilitates straightforward parsing and processing of messages by both client and server software.  

3.3.2. Numeric Replies:

A significant feature of the IRC protocol is that most commands initiated by clients result in a reply from the server, often in the form of a three-digit numeric code. These numeric replies serve a dual purpose, indicating both error conditions and providing normal responses to successful commands. A numeric reply must always include the prefix of the sending server, the three-digit numeric code itself, and the target of the reply, which is typically the client that initiated the command. Importantly, numeric replies can only originate from servers and never from clients. The use of these standardized numeric replies provides a structured and easily parsable method for servers to communicate the outcome of client requests. This allows client software to programmatically handle various scenarios, including errors, and to interpret the information returned by the server.  

4. IRC Concepts:

4.1. One-to-one communication:

The IRC protocol supports direct, one-to-one communication between clients. To facilitate this, all servers within the IRC network are required to have the capability to route messages along the spanning tree topology to reach any client connected to the network. The RFC provides examples to illustrate how messages traverse the network in a sample IRC setup. The reliance on the spanning tree for routing messages between individual clients implies that a message might need to pass through multiple intermediate servers before reaching its intended recipient. This multi-hop routing can potentially introduce latency in the communication, highlighting the impact of the network's underlying topology on the efficiency of message delivery.  

4.2. One-to-many:

IRC offers several mechanisms for sending messages to multiple recipients simultaneously.  

4.2.1. To a list:

One method involves sending a message to a predefined list of users. In this approach, the server takes the responsibility of dispatching a separate copy of the message to each user in the specified list. While straightforward, this method can become inefficient, particularly when dealing with a large number of recipients, as it increases the processing load on the server and consumes more network bandwidth due to the replication of the message. This suggests a potential limitation in scalability for very large-scale multi-user messaging using this particular approach.  

4.2.2. To a group (channel):

A more efficient way to communicate with multiple users in IRC is through channels, which function as multicast groups. When a message is sent to a channel, it is initially routed only to those servers that have at least one user currently joined to that channel. Subsequently, these servers distribute the message to all the clients connected to them that are members of the channel. The RFC includes examples to clearly illustrate this distribution process. The channel mechanism provides a significantly more efficient method for group communication compared to sending individual messages to each user. By leveraging the server network to distribute messages only where they are needed, IRC optimizes bandwidth usage and reduces server load, underscoring the central role of channels in its architecture for facilitating group interactions.  

4.2.3. To a host/server mask:

IRC also provides a mechanism that allows IRC operators to send messages to users based on their host or server information matching a given mask. This capability provides a powerful tool for network administrators to make network-wide announcements or to target specific groups of users based on their network location or the server they are connected to. This feature highlights the administrative capabilities built into the IRC protocol, allowing for focused communication with specific segments of the user base.  

4.3. One to all:

IRC addresses the need for broadcast messages intended for all clients or servers within the network.  

4.3.1. Client to Client:

The protocol does not define a single, direct message that a regular client can send to broadcast to all other clients on the entire network. This absence of a direct client-to-all broadcast mechanism likely stems from a design consideration to prevent potential abuse, such as spamming, and to avoid overwhelming the network with excessive traffic. This suggests a deliberate focus on more controlled and targeted forms of communication within the protocol.  

4.3.2. Clients to Server:

Commands initiated by clients that result in a change of state information within the network, such as a user joining or leaving a channel, or changes to user or channel modes, are typically propagated to all servers in the network by default. This ensures that all servers maintain a consistent view of the network's current state, including channel memberships and user settings, across the distributed IRC infrastructure. The automatic propagation of these state-changing commands is crucial for the proper functioning of the network and highlights its distributed nature.  

4.3.3. Server to Server:

Similarly, most messages originating from servers that affect users, channels, or the servers themselves are broadcast to all other connected servers. This server-to-server broadcasting is essential for maintaining network-wide consistency regarding information about users, channels, and the overall network topology. For the IRC network to operate correctly and provide a coherent experience to users, all servers must have access to the latest information about changes occurring throughout the network.  

5. Message Details:

The IRC protocol defines a variety of commands that clients and servers can use to interact with each other. These commands can be broadly categorized based on their function. This categorization reveals the logical organization of the protocol and the different stages and aspects of user interaction and network management.  

5.1. Connection Registration:

A set of commands is specifically used to establish a connection with an IRC server. These include PASS for providing a connection password, NICK for registering the client's desired nickname, and USER for providing the client's username, hostname, and real name. For server-to-server connections, the SERVER command is used, while OPER is used by clients to authenticate as an IRC operator. To terminate a connection, clients use the QUIT command, and servers use SQUIT to disconnect from each other. The sequence of these commands, particularly for client registration (PASS, NICK, USER), illustrates the necessary steps a client must take to successfully join the IRC network. The separate commands for server connections (SERVER and SQUIT) underscore the distinct nature of communication and management between servers.  

5.2. Channel Operations:

Several commands are dedicated to managing channels within the IRC network. The JOIN command allows a user to enter a specific channel, while PART enables a user to leave a channel they are currently in. The MODE command is used to change various settings or modes for both channels and individual users. The TOPIC command allows users to set or view the topic of discussion for a channel. To see a list of users currently in a channel, the NAMES command is used, and LIST retrieves a list of available channels on the connected server. The INVITE command allows a user to invite another user to join a channel, and KICK is used to remove a user from a channel. These commands represent the core functionalities for interacting with and managing channels, which are fundamental to the IRC user experience. The diverse set of commands allows for dynamic control over channel membership and various channel settings.  

5.3. Server Queries and Commands:

A set of commands allows users and operators to query information about servers and to issue commands to servers. The VERSION command retrieves the software version of the server, while STATS provides various server statistics. LINKS lists the servers known to the currently connected server, and TIME returns the local time of the server. The CONNECT command can be used to force a server to establish a connection with another server, and TRACE allows for tracing the route to a specific server in the network. Finally, ADMIN retrieves information about the server administrator, and INFO provides general information about the server. These commands are primarily utilized for network administration and monitoring purposes, enabling operators to gather crucial information about the network's status, topology, and configuration. They highlight the management and diagnostic aspects of the IRC protocol.  

5.4. Sending Messages:

The IRC protocol provides commands for sending text-based messages between users and to channels. The PRIVMSG command is used to send a private message to a specific user or to send a message to a channel. The NOTICE command functions similarly to PRIVMSG but is typically used for automated messages or notifications that do not necessarily require an automatic reply. These two commands are the fundamental means for text-based communication within the IRC network, with PRIVMSG serving as the primary command for both direct and channel-based conversations, and NOTICE providing a mechanism for conveying informational messages.  

5.5. User-based Queries:

Several commands allow users and operators to query information about other users on the network. The WHO command retrieves a list of users matching specified criteria, such as users in a particular channel or with a specific nickname pattern. The WHOIS command provides detailed information about a specific user, including their nickname, username, hostname, real name, and the server they are connected to. The WHOWAS command allows querying information about a user who has recently disconnected from the network or changed their nickname. These commands are essential for user discovery and for gaining context about participants within the IRC network.  

5.6. Miscellaneous Messages:

A collection of other essential commands serve various purposes in maintaining the health and functionality of the IRC network. The KILL command, which can only be used by operators, forcibly terminates a client's connection to the server. The PING command is used to test the liveness of a connection by sending a request to which the recipient should reply. The PONG command is the standard reply to a PING command, indicating that the connection is still active. Finally, the ERROR command is used by servers to report serious errors to connected clients or other servers. These commands play a vital role in ensuring the stability and reliability of the IRC network. PING and PONG are crucial for detecting and handling broken connections, while ERROR provides a mechanism for communicating critical issues. The KILL command, once again, highlights the administrative control wielded by IRC operators.  

6. Optional Messages:

The IRC protocol also defines a set of messages that are not strictly required for a basic server implementation but can provide additional features and functionality. These optional commands include AWAY, which allows users to set an "away" message to inform others that they are currently unavailable. REHASH instructs the server to re-read its configuration file , while RESTART forces the server to restart. SUMMON was intended to request a user to join IRC , and USERS provides a list of users logged into the server. OPERWALL allows operators to send a message to all other online operators , and USERHOST can be used to retrieve the host information for a given list of nicknames. Finally, ISON checks if a specified list of nicknames are currently online. The inclusion of these optional messages suggests a degree of flexibility and extensibility within the IRC protocol, allowing for the development of more feature-rich server implementations and enhanced user experiences.  

7. Replies:

Numeric replies are a fundamental aspect of the IRC protocol, providing a standardized way for servers to respond to commands issued by clients. These replies are categorized based on their purpose and the range of numeric codes they use.  

7.1. Error Replies:

Error replies are indicated by numeric codes in the 400s and 500s. These codes signal various error conditions that may have occurred as a result of a client's command. Examples of error replies include ERR_NOSUCHNICK (401), indicating that the specified nickname does not exist, ERR_NOTONCHANNEL (442), signifying that the user is not a member of the target channel, and ERR_NEEDMOREPARAMS (461), which indicates that the client did not provide enough parameters for the command. The comprehensive range of error codes allows client software to identify specific problems and potentially provide informative feedback to the user.  

7.2. Command Responses:

Successful command execution or requests for information result in command responses, which are indicated by numeric codes in the 200s and 300s. Examples of command responses include RPL_WELCOME (001), which is typically the first message sent by a server to a client upon successful registration, RPL_TOPIC (332), which contains the topic of a specified channel, and RPL_NAMREPLY (353), which provides a list of the users currently in a channel. These positive response codes are crucial for confirming that client actions were successful and for delivering the requested data back to the client.  

7.3. Reserved Numerics:

The IRC protocol also includes a set of reserved numeric codes. These are codes that may no longer be in active use, are specifically reserved for future extensions to the protocol, or are part of non-standard, server-specific features. The existence of these reserved numerics demonstrates a degree of foresight in the protocol's design, allowing for potential future expansion and the incorporation of custom features without conflicting with the core set of reply codes.  

8. Client and Server Authentication:

The IRC protocol incorporates several mechanisms for authenticating both clients and servers connecting to the network. These mechanisms include performing IP address to hostname lookups to verify the origin of a connection, conducting reverse checks to ensure the hostname resolves back to the originating IP address, and requiring password verification for certain connections. Additionally, the protocol mentions the use of an "Identity Server" (IDENT) to further verify the username of a client connecting to the server. The implementation of these multiple authentication layers highlights the importance placed on verifying the identity of participants within the IRC network, contributing to the overall security and trustworthiness of the communication environment.  

9. Current Implementations Details:

RFC 1459 also delves into implementation-specific details that are relevant for developers looking to build IRC servers. It specifies that the protocol typically uses TCP as its underlying network protocol. The document discusses how servers should handle command parsing and the use of send queues for managing outgoing messages. It also describes the mechanism for detecting and handling broken connections using the PING command for connection liveness detection. The process of establishing both server-client and server-server connections is outlined, along with the methods for exchanging state information between interconnected servers. Further implementation details include how servers should handle the termination of connections, track nickname changes, and implement flood control mechanisms to prevent abuse from clients. The RFC also touches upon the use of non-blocking lookups for DNS and IDENT, the structure of server configuration files, and considerations for channel membership limits. This section provides valuable practical guidance for developers, addressing the technical challenges involved in implementing a robust and functional IRC server. The inclusion of topics like flood control underscores the need to consider network stability and prevent malicious behavior in server implementations.  

10. Current Problems:

While RFC 1459 laid the foundation for a widely used communication protocol, it also acknowledges certain limitations and problems inherent in its design. One significant challenge is scalability in very large IRC networks, where the spanning tree topology and the need to propagate state changes across all servers can lead to performance bottlenecks. The protocol also faces difficulties in ensuring the uniqueness of nicknames and channel names across the entire network, leading to potential collisions and user confusion. Algorithmic inefficiencies in certain server operations are also noted as areas for improvement. Furthermore, the RFC points out the lack of robust database consistency checks between servers, which can sometimes lead to inconsistencies in network state. The existence of race conditions in certain scenarios is also mentioned as a potential issue. Recognizing these problems within the original specification is crucial for understanding the evolution of IRC and the development of subsequent extensions or alternative protocols aimed at addressing these limitations.  

11. Support and Availability:

RFC 1459 provides information on resources available to those interested in IRC. It mentions the existence of mailing lists dedicated to discussions about IRC-related topics, serving as a platform for developers, administrators, and users to exchange information and seek support. The document also points to locations where software implementations of IRC clients and servers can be found. Additionally, it identifies the relevant newsgroup for IRC-related discussions. The inclusion of this information indicates the presence of an active community and readily available resources for individuals and organizations looking to engage with the IRC protocol, whether for development, deployment, or general use.  

12. Security Considerations:

The RFC briefly addresses security considerations relevant to the IRC protocol, referencing specific sections within the document where security aspects are discussed. While not extensively detailed in this particular RFC, the inclusion of a dedicated section, even if brief, highlights the importance of security in the design and implementation of any network communication protocol. The reference to specific sections suggests that security aspects are integrated within the technical specifications of the protocol.  

Key Takeaways and Emerging Themes:

The analysis of RFC 1459 reveals that the Internet Relay Chat protocol, as initially defined, is a foundational text-based communication system characterized by a client-server architecture and a spanning tree network topology. The protocol relies on a structured message format, utilizing commands and numeric replies for communication and control between clients and servers. Channels serve as the primary mechanism for facilitating group communication, with channel operators playing a crucial role in managing these virtual spaces. The protocol incorporates features for user identification, authentication, and network administration, reflecting the requirements of a multi-user, distributed communication system. However, the RFC also acknowledges inherent limitations, such as scalability challenges in large networks and the absence of strong consistency mechanisms, which have likely driven the development of subsequent extensions and alternative protocols. Despite its age, the continued availability of support resources suggests the enduring relevance and ongoing use of the IRC protocol in various contexts.
## Project Plan and Timeline

To ensure timely completion of the project, here is a proposed timeline of 15-25 days, breaking down tasks for each part of the project.

### **Week 1: Days 1-7**
- **Day 1-2:**
  - Setup project repository and basic C++ skeleton.
  - Core server setup (socket creation, binding, listening).
- **Day 3-5:**
  - Implement `poll()` loop for handling multiple clients.
  - Handle client connection management (read/write socket data).
- **Day 6-7:**
  - Implement connection timeout handling and clean disconnections.
  - Ensure graceful shutdown of the server.

### **Week 2: Days 8-14**
- **Day 8-10:**
  - Implement basic command parsing (`NICK`, `USER`, `PASS`).
  - Create command handlers for `JOIN`, `PART`, and `PRIVMSG`.
- **Day 11-13:**
  - Implement the error handling and numeric replies for invalid commands.
  - Add basic response formatting for IRC server responses.
- **Day 14:**
  - Begin integrating channel management (e.g., creating channels on `JOIN`).

### **Week 3: Days 15-21**
- **Day 15-17:**
  - Continue channel management (`TOPIC`, `MODE`, `KICK`, etc.).
  - Implement user management (nickname conflicts, user modes).
- **Day 18-19:**
  - Complete channel and user state management.
  - Handle kick and ban functionality.
- **Day 20-21:**
  - Test entire system for bugs and issues, focusing on edge cases (e.g., invalid commands, disconnections).

### **Week 4: Days 22-25**
- **Day 22-23:**
  - Final testing and debugging of the entire server.
  - Address any remaining bugs or performance issues.
- **Day 24:**
  - Write documentation (README, comments in the code, etc.).
- **Day 25:**
  - Final review, code cleanup, and project submission.

---

## Final Notes

This project will help you understand the complexities of network programming, concurrency, and handling multiple clients at once. Each team member will have their own distinct responsibilities but will need to collaborate closely to integrate the individual parts into a fully functional IRC server.

Make sure to communicate regularly, share code frequently, and test thoroughly to ensure the server works as expected.

---

Good luck, a chabiba!
