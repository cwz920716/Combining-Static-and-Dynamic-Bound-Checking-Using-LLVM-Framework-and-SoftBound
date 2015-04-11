
build/output:     file format elf64-x86-64


Disassembly of section .init:

0000000000400418 <_init>:
  400418:	48 83 ec 08          	sub    $0x8,%rsp
  40041c:	48 8b 05 d5 0b 20 00 	mov    0x200bd5(%rip),%rax        # 600ff8 <_DYNAMIC+0x1d0>
  400423:	48 85 c0             	test   %rax,%rax
  400426:	74 05                	je     40042d <_init+0x15>
  400428:	e8 43 00 00 00       	callq  400470 <__gmon_start__@plt>
  40042d:	48 83 c4 08          	add    $0x8,%rsp
  400431:	c3                   	retq   

Disassembly of section .plt:

0000000000400440 <printf@plt-0x10>:
  400440:	ff 35 c2 0b 20 00    	pushq  0x200bc2(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  400446:	ff 25 c4 0b 20 00    	jmpq   *0x200bc4(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  40044c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400450 <printf@plt>:
  400450:	ff 25 c2 0b 20 00    	jmpq   *0x200bc2(%rip)        # 601018 <_GLOBAL_OFFSET_TABLE_+0x18>
  400456:	68 00 00 00 00       	pushq  $0x0
  40045b:	e9 e0 ff ff ff       	jmpq   400440 <_init+0x28>

0000000000400460 <__libc_start_main@plt>:
  400460:	ff 25 ba 0b 20 00    	jmpq   *0x200bba(%rip)        # 601020 <_GLOBAL_OFFSET_TABLE_+0x20>
  400466:	68 01 00 00 00       	pushq  $0x1
  40046b:	e9 d0 ff ff ff       	jmpq   400440 <_init+0x28>

0000000000400470 <__gmon_start__@plt>:
  400470:	ff 25 b2 0b 20 00    	jmpq   *0x200bb2(%rip)        # 601028 <_GLOBAL_OFFSET_TABLE_+0x28>
  400476:	68 02 00 00 00       	pushq  $0x2
  40047b:	e9 c0 ff ff ff       	jmpq   400440 <_init+0x28>

0000000000400480 <malloc@plt>:
  400480:	ff 25 aa 0b 20 00    	jmpq   *0x200baa(%rip)        # 601030 <_GLOBAL_OFFSET_TABLE_+0x30>
  400486:	68 03 00 00 00       	pushq  $0x3
  40048b:	e9 b0 ff ff ff       	jmpq   400440 <_init+0x28>

Disassembly of section .text:

0000000000400490 <_start>:
  400490:	31 ed                	xor    %ebp,%ebp
  400492:	49 89 d1             	mov    %rdx,%r9
  400495:	5e                   	pop    %rsi
  400496:	48 89 e2             	mov    %rsp,%rdx
  400499:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  40049d:	50                   	push   %rax
  40049e:	54                   	push   %rsp
  40049f:	49 c7 c0 c0 08 40 00 	mov    $0x4008c0,%r8
  4004a6:	48 c7 c1 50 08 40 00 	mov    $0x400850,%rcx
  4004ad:	48 c7 c7 c0 05 40 00 	mov    $0x4005c0,%rdi
  4004b4:	e8 a7 ff ff ff       	callq  400460 <__libc_start_main@plt>
  4004b9:	f4                   	hlt    
  4004ba:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

00000000004004c0 <deregister_tm_clones>:
  4004c0:	b8 4f 10 60 00       	mov    $0x60104f,%eax
  4004c5:	55                   	push   %rbp
  4004c6:	48 2d 48 10 60 00    	sub    $0x601048,%rax
  4004cc:	48 83 f8 0e          	cmp    $0xe,%rax
  4004d0:	48 89 e5             	mov    %rsp,%rbp
  4004d3:	77 02                	ja     4004d7 <deregister_tm_clones+0x17>
  4004d5:	5d                   	pop    %rbp
  4004d6:	c3                   	retq   
  4004d7:	b8 00 00 00 00       	mov    $0x0,%eax
  4004dc:	48 85 c0             	test   %rax,%rax
  4004df:	74 f4                	je     4004d5 <deregister_tm_clones+0x15>
  4004e1:	5d                   	pop    %rbp
  4004e2:	bf 48 10 60 00       	mov    $0x601048,%edi
  4004e7:	ff e0                	jmpq   *%rax
  4004e9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000004004f0 <register_tm_clones>:
  4004f0:	b8 48 10 60 00       	mov    $0x601048,%eax
  4004f5:	55                   	push   %rbp
  4004f6:	48 2d 48 10 60 00    	sub    $0x601048,%rax
  4004fc:	48 c1 f8 03          	sar    $0x3,%rax
  400500:	48 89 e5             	mov    %rsp,%rbp
  400503:	48 89 c2             	mov    %rax,%rdx
  400506:	48 c1 ea 3f          	shr    $0x3f,%rdx
  40050a:	48 01 d0             	add    %rdx,%rax
  40050d:	48 d1 f8             	sar    %rax
  400510:	75 02                	jne    400514 <register_tm_clones+0x24>
  400512:	5d                   	pop    %rbp
  400513:	c3                   	retq   
  400514:	ba 00 00 00 00       	mov    $0x0,%edx
  400519:	48 85 d2             	test   %rdx,%rdx
  40051c:	74 f4                	je     400512 <register_tm_clones+0x22>
  40051e:	5d                   	pop    %rbp
  40051f:	48 89 c6             	mov    %rax,%rsi
  400522:	bf 48 10 60 00       	mov    $0x601048,%edi
  400527:	ff e2                	jmpq   *%rdx
  400529:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400530 <__do_global_dtors_aux>:
  400530:	80 3d 11 0b 20 00 00 	cmpb   $0x0,0x200b11(%rip)        # 601048 <__TMC_END__>
  400537:	75 11                	jne    40054a <__do_global_dtors_aux+0x1a>
  400539:	55                   	push   %rbp
  40053a:	48 89 e5             	mov    %rsp,%rbp
  40053d:	e8 7e ff ff ff       	callq  4004c0 <deregister_tm_clones>
  400542:	5d                   	pop    %rbp
  400543:	c6 05 fe 0a 20 00 01 	movb   $0x1,0x200afe(%rip)        # 601048 <__TMC_END__>
  40054a:	f3 c3                	repz retq 
  40054c:	0f 1f 40 00          	nopl   0x0(%rax)

0000000000400550 <frame_dummy>:
  400550:	48 83 3d c8 08 20 00 	cmpq   $0x0,0x2008c8(%rip)        # 600e20 <__JCR_END__>
  400557:	00 
  400558:	74 1e                	je     400578 <frame_dummy+0x28>
  40055a:	b8 00 00 00 00       	mov    $0x0,%eax
  40055f:	48 85 c0             	test   %rax,%rax
  400562:	74 14                	je     400578 <frame_dummy+0x28>
  400564:	55                   	push   %rbp
  400565:	bf 20 0e 60 00       	mov    $0x600e20,%edi
  40056a:	48 89 e5             	mov    %rsp,%rbp
  40056d:	ff d0                	callq  *%rax
  40056f:	5d                   	pop    %rbp
  400570:	e9 7b ff ff ff       	jmpq   4004f0 <register_tm_clones>
  400575:	0f 1f 00             	nopl   (%rax)
  400578:	e9 73 ff ff ff       	jmpq   4004f0 <register_tm_clones>
  40057d:	0f 1f 00             	nopl   (%rax)

0000000000400580 <f>:
  400580:	55                   	push   %rbp
  400581:	48 89 e5             	mov    %rsp,%rbp
  400584:	41 56                	push   %r14
  400586:	53                   	push   %rbx
  400587:	4c 63 f7             	movslq %edi,%r14
  40058a:	4c 89 f3             	mov    %r14,%rbx
  40058d:	48 c1 e3 04          	shl    $0x4,%rbx
  400591:	48 89 df             	mov    %rbx,%rdi
  400594:	e8 e7 fe ff ff       	callq  400480 <malloc@plt>
  400599:	48 89 c1             	mov    %rax,%rcx
  40059c:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  4005a1:	31 c0                	xor    %eax,%eax
  4005a3:	48 89 ce             	mov    %rcx,%rsi
  4005a6:	48 89 da             	mov    %rbx,%rdx
  4005a9:	e8 a2 fe ff ff       	callq  400450 <printf@plt>
  4005ae:	44 89 f0             	mov    %r14d,%eax
  4005b1:	5b                   	pop    %rbx
  4005b2:	41 5e                	pop    %r14
  4005b4:	5d                   	pop    %rbp
  4005b5:	c3                   	retq   
  4005b6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  4005bd:	00 00 00 

00000000004005c0 <main>:
  4005c0:	55                   	push   %rbp
  4005c1:	48 89 e5             	mov    %rsp,%rbp
  4005c4:	48 83 ec 30          	sub    $0x30,%rsp
  4005c8:	48 8d 75 d0          	lea    -0x30(%rbp),%rsi
  4005cc:	bf d4 08 40 00       	mov    $0x4008d4,%edi
  4005d1:	ba 0a 00 00 00       	mov    $0xa,%edx
  4005d6:	b9 20 00 00 00       	mov    $0x20,%ecx
  4005db:	41 b8 10 00 00 00    	mov    $0x10,%r8d
  4005e1:	31 c0                	xor    %eax,%eax
  4005e3:	e8 68 fe ff ff       	callq  400450 <printf@plt>
  4005e8:	31 ff                	xor    %edi,%edi
  4005ea:	e8 91 fe ff ff       	callq  400480 <malloc@plt>
  4005ef:	48 89 c1             	mov    %rax,%rcx
  4005f2:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  4005f7:	31 d2                	xor    %edx,%edx
  4005f9:	31 c0                	xor    %eax,%eax
  4005fb:	48 89 ce             	mov    %rcx,%rsi
  4005fe:	e8 4d fe ff ff       	callq  400450 <printf@plt>
  400603:	c7 45 d0 00 00 00 00 	movl   $0x0,-0x30(%rbp)
  40060a:	bf 10 00 00 00       	mov    $0x10,%edi
  40060f:	e8 6c fe ff ff       	callq  400480 <malloc@plt>
  400614:	48 89 c1             	mov    %rax,%rcx
  400617:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  40061c:	ba 10 00 00 00       	mov    $0x10,%edx
  400621:	31 c0                	xor    %eax,%eax
  400623:	48 89 ce             	mov    %rcx,%rsi
  400626:	e8 25 fe ff ff       	callq  400450 <printf@plt>
  40062b:	c7 45 d4 01 00 00 00 	movl   $0x1,-0x2c(%rbp)
  400632:	bf 20 00 00 00       	mov    $0x20,%edi
  400637:	e8 44 fe ff ff       	callq  400480 <malloc@plt>
  40063c:	48 89 c1             	mov    %rax,%rcx
  40063f:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  400644:	ba 20 00 00 00       	mov    $0x20,%edx
  400649:	31 c0                	xor    %eax,%eax
  40064b:	48 89 ce             	mov    %rcx,%rsi
  40064e:	e8 fd fd ff ff       	callq  400450 <printf@plt>
  400653:	c7 45 d8 02 00 00 00 	movl   $0x2,-0x28(%rbp)
  40065a:	bf 30 00 00 00       	mov    $0x30,%edi
  40065f:	e8 1c fe ff ff       	callq  400480 <malloc@plt>
  400664:	48 89 c1             	mov    %rax,%rcx
  400667:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  40066c:	ba 30 00 00 00       	mov    $0x30,%edx
  400671:	31 c0                	xor    %eax,%eax
  400673:	48 89 ce             	mov    %rcx,%rsi
  400676:	e8 d5 fd ff ff       	callq  400450 <printf@plt>
  40067b:	c7 45 dc 03 00 00 00 	movl   $0x3,-0x24(%rbp)
  400682:	bf 40 00 00 00       	mov    $0x40,%edi
  400687:	e8 f4 fd ff ff       	callq  400480 <malloc@plt>
  40068c:	48 89 c1             	mov    %rax,%rcx
  40068f:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  400694:	ba 40 00 00 00       	mov    $0x40,%edx
  400699:	31 c0                	xor    %eax,%eax
  40069b:	48 89 ce             	mov    %rcx,%rsi
  40069e:	e8 ad fd ff ff       	callq  400450 <printf@plt>
  4006a3:	c7 45 e0 04 00 00 00 	movl   $0x4,-0x20(%rbp)
  4006aa:	bf 50 00 00 00       	mov    $0x50,%edi
  4006af:	e8 cc fd ff ff       	callq  400480 <malloc@plt>
  4006b4:	48 89 c1             	mov    %rax,%rcx
  4006b7:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  4006bc:	ba 50 00 00 00       	mov    $0x50,%edx
  4006c1:	31 c0                	xor    %eax,%eax
  4006c3:	48 89 ce             	mov    %rcx,%rsi
  4006c6:	e8 85 fd ff ff       	callq  400450 <printf@plt>
  4006cb:	c7 45 e4 05 00 00 00 	movl   $0x5,-0x1c(%rbp)
  4006d2:	bf 60 00 00 00       	mov    $0x60,%edi
  4006d7:	e8 a4 fd ff ff       	callq  400480 <malloc@plt>
  4006dc:	48 89 c1             	mov    %rax,%rcx
  4006df:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  4006e4:	ba 60 00 00 00       	mov    $0x60,%edx
  4006e9:	31 c0                	xor    %eax,%eax
  4006eb:	48 89 ce             	mov    %rcx,%rsi
  4006ee:	e8 5d fd ff ff       	callq  400450 <printf@plt>
  4006f3:	c7 45 e8 06 00 00 00 	movl   $0x6,-0x18(%rbp)
  4006fa:	bf 70 00 00 00       	mov    $0x70,%edi
  4006ff:	e8 7c fd ff ff       	callq  400480 <malloc@plt>
  400704:	48 89 c1             	mov    %rax,%rcx
  400707:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  40070c:	ba 70 00 00 00       	mov    $0x70,%edx
  400711:	31 c0                	xor    %eax,%eax
  400713:	48 89 ce             	mov    %rcx,%rsi
  400716:	e8 35 fd ff ff       	callq  400450 <printf@plt>
  40071b:	c7 45 ec 07 00 00 00 	movl   $0x7,-0x14(%rbp)
  400722:	bf 80 00 00 00       	mov    $0x80,%edi
  400727:	e8 54 fd ff ff       	callq  400480 <malloc@plt>
  40072c:	48 89 c1             	mov    %rax,%rcx
  40072f:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  400734:	ba 80 00 00 00       	mov    $0x80,%edx
  400739:	31 c0                	xor    %eax,%eax
  40073b:	48 89 ce             	mov    %rcx,%rsi
  40073e:	e8 0d fd ff ff       	callq  400450 <printf@plt>
  400743:	c7 45 f0 08 00 00 00 	movl   $0x8,-0x10(%rbp)
  40074a:	bf 90 00 00 00       	mov    $0x90,%edi
  40074f:	e8 2c fd ff ff       	callq  400480 <malloc@plt>
  400754:	48 89 c1             	mov    %rax,%rcx
  400757:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  40075c:	ba 90 00 00 00       	mov    $0x90,%edx
  400761:	31 c0                	xor    %eax,%eax
  400763:	48 89 ce             	mov    %rcx,%rsi
  400766:	e8 e5 fc ff ff       	callq  400450 <printf@plt>
  40076b:	c7 45 f4 09 00 00 00 	movl   $0x9,-0xc(%rbp)
  400772:	bf d0 02 00 00       	mov    $0x2d0,%edi
  400777:	e8 04 fd ff ff       	callq  400480 <malloc@plt>
  40077c:	48 89 c1             	mov    %rax,%rcx
  40077f:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  400784:	ba d0 02 00 00       	mov    $0x2d0,%edx
  400789:	31 c0                	xor    %eax,%eax
  40078b:	48 89 ce             	mov    %rcx,%rsi
  40078e:	e8 bd fc ff ff       	callq  400450 <printf@plt>
  400793:	b8 2d 00 00 00       	mov    $0x2d,%eax
  400798:	48 83 c4 30          	add    $0x30,%rsp
  40079c:	5d                   	pop    %rbp
  40079d:	c3                   	retq   
  40079e:	66 90                	xchg   %ax,%ax

00000000004007a0 <__track_stack_allocation>:
  4007a0:	55                   	push   %rbp
  4007a1:	48 89 e5             	mov    %rsp,%rbp
  4007a4:	49 89 c8             	mov    %rcx,%r8
  4007a7:	48 89 d1             	mov    %rdx,%rcx
  4007aa:	49 89 f9             	mov    %rdi,%r9
  4007ad:	bf d4 08 40 00       	mov    $0x4008d4,%edi
  4007b2:	31 c0                	xor    %eax,%eax
  4007b4:	48 89 f2             	mov    %rsi,%rdx
  4007b7:	4c 89 ce             	mov    %r9,%rsi
  4007ba:	5d                   	pop    %rbp
  4007bb:	e9 90 fc ff ff       	jmpq   400450 <printf@plt>

00000000004007c0 <__track_heap_allocation>:
  4007c0:	55                   	push   %rbp
  4007c1:	48 89 e5             	mov    %rsp,%rbp
  4007c4:	48 89 f1             	mov    %rsi,%rcx
  4007c7:	48 89 fa             	mov    %rdi,%rdx
  4007ca:	bf f7 08 40 00       	mov    $0x4008f7,%edi
  4007cf:	31 c0                	xor    %eax,%eax
  4007d1:	48 89 d6             	mov    %rdx,%rsi
  4007d4:	48 89 ca             	mov    %rcx,%rdx
  4007d7:	5d                   	pop    %rbp
  4007d8:	e9 73 fc ff ff       	jmpq   400450 <printf@plt>

00000000004007dd <_Z24__track_stack_allocationPvjmm>:
  4007dd:	55                   	push   %rbp
  4007de:	48 89 e5             	mov    %rsp,%rbp
  4007e1:	48 83 ec 20          	sub    $0x20,%rsp
  4007e5:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  4007e9:	89 75 f4             	mov    %esi,-0xc(%rbp)
  4007ec:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
  4007f0:	48 89 4d e0          	mov    %rcx,-0x20(%rbp)
  4007f4:	48 8b 75 e0          	mov    -0x20(%rbp),%rsi
  4007f8:	48 8b 4d e8          	mov    -0x18(%rbp),%rcx
  4007fc:	8b 55 f4             	mov    -0xc(%rbp),%edx
  4007ff:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  400803:	49 89 f0             	mov    %rsi,%r8
  400806:	48 89 c6             	mov    %rax,%rsi
  400809:	48 8d 3d 08 01 00 00 	lea    0x108(%rip),%rdi        # 400918 <_IO_stdin_used+0x48>
  400810:	b8 00 00 00 00       	mov    $0x0,%eax
  400815:	e8 36 fc ff ff       	callq  400450 <printf@plt>
  40081a:	c9                   	leaveq 
  40081b:	c3                   	retq   

000000000040081c <_Z23__track_heap_allocationPvm>:
  40081c:	55                   	push   %rbp
  40081d:	48 89 e5             	mov    %rsp,%rbp
  400820:	48 83 ec 10          	sub    $0x10,%rsp
  400824:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  400828:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  40082c:	48 8b 55 f0          	mov    -0x10(%rbp),%rdx
  400830:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  400834:	48 89 c6             	mov    %rax,%rsi
  400837:	48 8d 3d fb 00 00 00 	lea    0xfb(%rip),%rdi        # 400939 <_IO_stdin_used+0x69>
  40083e:	b8 00 00 00 00       	mov    $0x0,%eax
  400843:	e8 08 fc ff ff       	callq  400450 <printf@plt>
  400848:	c9                   	leaveq 
  400849:	c3                   	retq   
  40084a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400850 <__libc_csu_init>:
  400850:	41 57                	push   %r15
  400852:	41 89 ff             	mov    %edi,%r15d
  400855:	41 56                	push   %r14
  400857:	49 89 f6             	mov    %rsi,%r14
  40085a:	41 55                	push   %r13
  40085c:	49 89 d5             	mov    %rdx,%r13
  40085f:	41 54                	push   %r12
  400861:	4c 8d 25 a8 05 20 00 	lea    0x2005a8(%rip),%r12        # 600e10 <__frame_dummy_init_array_entry>
  400868:	55                   	push   %rbp
  400869:	48 8d 2d a8 05 20 00 	lea    0x2005a8(%rip),%rbp        # 600e18 <__init_array_end>
  400870:	53                   	push   %rbx
  400871:	4c 29 e5             	sub    %r12,%rbp
  400874:	31 db                	xor    %ebx,%ebx
  400876:	48 c1 fd 03          	sar    $0x3,%rbp
  40087a:	48 83 ec 08          	sub    $0x8,%rsp
  40087e:	e8 95 fb ff ff       	callq  400418 <_init>
  400883:	48 85 ed             	test   %rbp,%rbp
  400886:	74 1e                	je     4008a6 <__libc_csu_init+0x56>
  400888:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  40088f:	00 
  400890:	4c 89 ea             	mov    %r13,%rdx
  400893:	4c 89 f6             	mov    %r14,%rsi
  400896:	44 89 ff             	mov    %r15d,%edi
  400899:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  40089d:	48 83 c3 01          	add    $0x1,%rbx
  4008a1:	48 39 eb             	cmp    %rbp,%rbx
  4008a4:	75 ea                	jne    400890 <__libc_csu_init+0x40>
  4008a6:	48 83 c4 08          	add    $0x8,%rsp
  4008aa:	5b                   	pop    %rbx
  4008ab:	5d                   	pop    %rbp
  4008ac:	41 5c                	pop    %r12
  4008ae:	41 5d                	pop    %r13
  4008b0:	41 5e                	pop    %r14
  4008b2:	41 5f                	pop    %r15
  4008b4:	c3                   	retq   
  4008b5:	66 66 2e 0f 1f 84 00 	data32 nopw %cs:0x0(%rax,%rax,1)
  4008bc:	00 00 00 00 

00000000004008c0 <__libc_csu_fini>:
  4008c0:	f3 c3                	repz retq 

Disassembly of section .fini:

00000000004008c4 <_fini>:
  4008c4:	48 83 ec 08          	sub    $0x8,%rsp
  4008c8:	48 83 c4 08          	add    $0x8,%rsp
  4008cc:	c3                   	retq   
