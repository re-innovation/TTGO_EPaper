#include "quote.h"

#include <stdio.h>
#include <Arduino.h>


String parseQuote(String _payload)
{
  // Here we want to strip out the quote and the author
  // This is in the format "quote (author)"
  // So parse the payload to find quote and author

  int openingBracket = _payload.indexOf('(');
  int closingBracket = _payload.indexOf(')');
  String _quote = "";
  // Need to check that there was an author
  if (openingBracket > 0 && closingBracket > 0)
  {
    // This means the author is valid
    // Need to strip out the author
    _quote = _payload.substring(0, openingBracket);
  }
  else
  {
    // This case there is no author
    _quote = _payload;

  }
  return (_quote);
}

String parseAuthor(String _payload)
{
  // Here we want to strip out the quote and the author
  // This is in the format "quote (author)"
  // So parse the payload to find quote and author

  int openingBracket = _payload.indexOf('(');
  int closingBracket = _payload.indexOf(')');

  String _author = "";

  // Need to check that there was an author
  if (openingBracket > 0 && closingBracket > 0)
  {
    // This means the author is valid
    // Need to strip out the author
    _author = _payload.substring(openingBracket + 1, closingBracket);
  }
  else
  {
    // This case there is no author
    _author = "Anon" ;
  }
  return (_author);
}
