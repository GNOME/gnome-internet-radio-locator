<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet
  version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns="http://www.w3.org/1999/xhtml">
  <xsl:output method="xml" indent="yes" encoding="UTF-8"/>
  <xsl:template match="/">
    <html>
      <body>
	<h2>Internet Radio Stations</h2>
	<table border="1">
	  <tr bgcolor="#9acd32">
	    <th style="text-align:left">Station</th>
	    <th style="text-align:left">Frequency</th>
	    <th style="text-align:left">Description</th>
	    <th style="text-align:left">Streams</th>
	  </tr>
	  <xsl:for-each select="gnome_internet_radio_locator/station">
	    <tr>
	      <td><a href="{uri}" target="_new"><xsl:value-of select="@name"/></a></td>
	      <td><xsl:value-of select="@band"/> in <xsl:value-of select="location"/></td>
	      <td><xsl:value-of select="description"/></td>
	      <td>
		<xsl:for-each select="stream">
		  <table>
		    <tr bgcolor="#9acd32" style="color: #ffffff">
		      <td style="text-align:left">MIME</td><td><xsl:value-of select="@mime"/></td>
		    </tr>
		    <tr bgcolor="#cd9a32" style="color: #ffffff">
		      <td style="text-align:left">URI</td><td><a href="{@uri}"><xsl:value-of select="@uri"/></a></td>
		    </tr>
		    <tr bgcolor="#cd329a" style="color: #ffffff">
		      <td style="text-align:left">Codec</td><td><xsl:value-of select="@codec"/></td>
		    </tr>
		    <tr bgcolor="#9a32cd" style="color: #ffffff">
		      <td style="text-align:left">Bitrate</td><td><xsl:value-of select="@bitrate"/></td>
		    </tr>
		    <tr bgcolor="#3232cd" style="color: #ffffff">
		      <td style="text-align:left">Channels</td><td><xsl:value-of select="@channels"/></td>
		    </tr>
		    <tr bgcolor="#9a329a" style="color: #ffffff">
		      <td style="text-align:left">Samplerate</td><td><xsl:value-of select="@samplerate"/></td>
		    </tr>
		  </table>
		  <br />
		</xsl:for-each>		   
	      </td>
	    </tr>
	  </xsl:for-each>
	</table>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
