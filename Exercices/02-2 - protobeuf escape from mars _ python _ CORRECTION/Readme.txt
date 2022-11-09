You're a captain who is lost on Mars. Write the captain code to contact the earth
ground control station to ask a rescue for your team and successfully received a
rescue date.

Good luck.

Mars to ground control station communication is defined through protobuf.
You can code the lost captain code in C or in python. The ground control station
code is written in C and compiled to be run on x86_64. msg_size must be
uint32_t. Ground control station accept only one client connection.

Following conversation is expected for the same rescue id :

=================================
   Mars   <------------>   Earth

  --> msg_size + FromMars: RescueDemand -->  
  <-- msg_size + FromEarth: RescueOffer <--
  --> msg_size + FromMars: RescueAck    -->
=================================

The success of your captain code will be confirmed if the ground control station
process is writting on stdout : "Rescue mission <id> started."
