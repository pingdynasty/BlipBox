<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE xsl:stylesheet [
<!ENTITY nbsp "&#160;">
]>
<xsl:stylesheet version="1.0"
		xmlns:ns="http://www.o-xml.org/lang/"
		xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  
  <xsl:output method="html" indent="yes"/>

  <xsl:template name="substitute">
    <xsl:param name="str"/>
    <xsl:choose>
      <xsl:when test="starts-with($str, ' ')">&nbsp;</xsl:when>
      <xsl:when test="starts-with($str, '&#9;')">&nbsp;&nbsp;&nbsp;&nbsp;</xsl:when>
      <xsl:when test="starts-with($str, '&#10;')"><br/></xsl:when>
      <xsl:when test="starts-with($str, '&#13;')"><br/></xsl:when>
      <xsl:otherwise>
	<xsl:value-of select="substring($str, 1, 1)"/>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:if test="string-length($str) &gt; 1">
      <xsl:call-template name="substitute">
	<xsl:with-param name="str" select="substring($str, 2)"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template match="html">
    <xsl:copy-of select="."/>
  </xsl:template>

  <xsl:template match="article">
    <html>
      <head>
	<title><xsl:value-of select="title"/></title>
	<style>
          a:link {color: #990000;text-decoration: none;}
          a:visited {color: #990000;text-decoration: none;}
          a:active {color: #808080;text-decoration: none;}
          a:hover {color: #000000;text-decoration: none;} 
          body { font-family:Tahoma, Geneva, Arial, Helvetica, Verdana, sans-serif; margin:0; background-color:#cbd3c7 ; }
<!--           body { font-family:Tahoma, Geneva, Arial, Helvetica, Verdana, sans-serif; margin:0; font-size:11px; color:#fff; background-color:#cbd3c7 ; text-align: center; } -->
<!--           body { font-family:Tahoma, Verdana, Geneva, Arial, Helvetica, sans-serif; } -->
          img { border: none }
          .header { position: absolute;  top: 30px; left: 180px; width: 520px;  }
          .sidebar { position: absolute;  top: 30px; left: 60px; width: 100px;  }
          .content { position: absolute; top: 80px; left: 180px; width: 520px; }
          .menubar { }

	  .example {font-size: small; font-family: 'Courier New', 'Courier'; }
	  .markup  {color: darkblue}
	  .element-name  {color: darkgreen}
	  .code-name {color: darkblue}
	  .attribute-name  {color: green}
	  .attribute-value  {color: green}
	  .text {  }

	  td.term {font-size: small; font-family: 'Courier New', 'Courier'}
           </style>
      </head>
      <body>
        <div class="header">
          <a href="http://blipbox.org"><img src="images/blipbox_logo_red_black_medium.png"/></a>
<!--           <a href="http://www.pingdynasty.com"><img src="images/pingdynasty.gif"/></a> -->
        </div>
        <div class="sidebar">
          <xsl:apply-templates select="document('sidebar.xml')/sidebar/*"/>
        </div>
        <div class="content">
          <xsl:apply-templates/>
        </div>
      </body>
    </html>
  </xsl:template>

  <xsl:template match="section">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="quote">
    <xsl:text>'</xsl:text>
    <xsl:apply-templates/>
    <xsl:text>'</xsl:text>
  </xsl:template>

  <xsl:template match="title">
    <p><b><xsl:value-of select="."/></b></p>
  </xsl:template>

  <xsl:template match="para">
    <p><xsl:apply-templates/></p>
  </xsl:template>

  <xsl:template match="variablelist">
    <table class="variablelist">
      <xsl:apply-templates/>
    </table>
  </xsl:template>

  <xsl:template match="varlistentry">
    <tr>
      <xsl:apply-templates/>
    </tr>
  </xsl:template>

  <xsl:template match="varlistentry/term">
    <td class="term"><xsl:value-of select="."/></td>
  </xsl:template>

  <xsl:template match="varlistentry/listitem">
    <td><xsl:apply-templates/></td>
  </xsl:template>

  <xsl:template match="itemizedlist">
    <ul>
      <xsl:apply-templates/>
    </ul>
  </xsl:template>

  <xsl:template match="itemizedlist/listitem">
    <li>
      <xsl:apply-templates/>
    </li>
  </xsl:template>

<!--   <xsl:template match="itemizedlist"> -->
<!--     <table class="itemizedlist"> -->
<!--       <xsl:apply-templates/> -->
<!--     </table> -->
<!--   </xsl:template> -->

<!--   <xsl:template match="itemizedlist/listitem"> -->
<!--     <tr> -->
<!--       <td>-</td> -->
<!--       <td> -->
<!-- 	<xsl:apply-templates/> -->
<!--       </td> -->
<!--     </tr> -->
<!--   </xsl:template> -->

  <xsl:template match="ulink">
    <a href="{@url}"><xsl:apply-templates/></a>
  </xsl:template>

  <xsl:template match="command">
    <i><xsl:apply-templates/></i>
  </xsl:template>

  <xsl:template match="emphasis">
    <i><xsl:apply-templates/></i>
  </xsl:template>

  <xsl:template match="example">
    <p><b>
      <xsl:text>Example </xsl:text>
      <xsl:value-of select="count(preceding::example) + 1"/>
      <xsl:text>: </xsl:text>
      <xsl:value-of select="title"/>
    </b></p>
    <xsl:apply-templates select="title/following-sibling::*"/>
  </xsl:template>

  <xsl:template match="informalexample">
    <xsl:apply-templates/>
  </xsl:template>

  <!-- images -->
  <xsl:template match="graphic">
      <img src="{@fileref}" alt="{@alt}"/>
  </xsl:template>

  <xsl:template match="mediaobject">
    <div class="mediaobject">
      <img src="{imageobject/imagedata/@fileref}">
      <xsl:attribute name="alt">
        <xsl:choose>
          <xsl:when test="textobject/phrase">
          <xsl:value-of select="textobject/phrase/text()"/></xsl:when>
          <xsl:otherwise><xsl:value-of select="caption/para/text()"/></xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      </img>
      <xsl:if test="caption">
        <div class="caption">
          <i><xsl:apply-templates select="caption/*"/></i>
        </div>
      </xsl:if>
    </div>
  </xsl:template>

  <!-- code examples -->
  <xsl:template match="programlisting">
    <div class="example">
      <xsl:apply-templates mode="listing"/>
    </div>
  </xsl:template>

  <xsl:template match="text()" mode="listing">
      <span class="text">
      <xsl:call-template name="substitute">
	<xsl:with-param name="str" select="."/>
      </xsl:call-template>
      </span>
  </xsl:template>

  <xsl:template match="ns:*" mode="name">
    <span class="code-name">
      <xsl:value-of select="name(.)"/>
    </span>    
  </xsl:template>

  <xsl:template match="*" mode="name">
    <span class="element-name">
      <xsl:value-of select="name(.)"/>
    </span>
  </xsl:template>

  <xsl:template match="*" mode="listing">
    <span class="markup">&lt;</span>
    <xsl:apply-templates select="." mode="name"/>
    <xsl:apply-templates select="@*" mode="listing"/>
    <span class="markup">/></span>
  </xsl:template>

  <xsl:template match="*[node()]" mode="listing">
    <span class="markup">&lt;</span>
    <xsl:apply-templates select="." mode="name"/>
    <xsl:apply-templates select="@*" mode="listing"/>
    <span class="markup">&gt;</span>
    <xsl:apply-templates mode="listing"/>
    <span class="markup">
      <xsl:text>&lt;/</xsl:text>
    </span>
    <xsl:apply-templates select="." mode="name"/>
    <span class="markup">
      <xsl:text>&gt;</xsl:text>
    </span>
  </xsl:template>

  <xsl:template match="@*" mode="listing">
    <span class="attribute-name">
      <xsl:text> </xsl:text>
      <xsl:value-of select="name(.)"/>
    </span>
    <span class="markup">="</span>
    <span class="attribute-value"><xsl:value-of select="."/></span>
    <span class="markup">"</span>
  </xsl:template>

  <xsl:template match="ns:*">
    <xsl:apply-templates select="." mode="listing"/>
  </xsl:template>

  <xsl:template match="*">
    <xsl:message>unmatched element: <xsl:value-of select="name(.)"/></xsl:message>
    <xsl:apply-templates/>
  </xsl:template>

</xsl:stylesheet>
