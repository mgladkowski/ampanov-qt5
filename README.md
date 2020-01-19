# Ampanov
## Stock trading robot using C++ QT, REST to Questrade, PHP and websocket front end.

### Problem : 

I learned technical analysis of stocks and options with Tradesmart University and gained a good understanding of securities trading and account management.  However my day job makes managing my own trades impractical.  I feel an insatiable desire to solve this - so much skill development and potential.

### Solution : 

Get a computer to do it for me.  I created a C++ QT console application, running as a service on Windows or Linux.  Via ZeroMQ and a websocket, a web front end is provided.  Connection to Questrade data is provided via REST, and the data is stored locally for fast analysis.  The C++ service runs a game loop during market hours, monitoring candles in real-time.  Buy/sell signals generate a trade candidate, with risk unit and account management parameters automatically computed.  Open orders and positions are monitored and stop-losses enforced.

### Ongoing development : 

Next steps for this project are to apply Machine Learning to the buy/sell signals.  Questrade removed the ability to execute order entry via API, so I will be moving the project to Interactive Brokers API as well.
